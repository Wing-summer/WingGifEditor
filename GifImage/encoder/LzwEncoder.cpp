//
// Created by succlz123 on 17-9-9.
//

#include "LzwEncoder.h"
#include <QObject>
#include <iostream>

using namespace blk;

static const int32_t MAX_STACK_SIZE = 4096;
static const int32_t BYTE_NUM = 256;
static const int32_t BLOCK_SIZE = 255;

LzwEncoder::~LzwEncoder() {
  for (auto &data : datas) {
    delete[] data;
  }
}

LzwEncoder::LzwEncoder(int32_t paddedColorCount) {
  numColors = paddedColorCount;
  current = new uint8_t[BLOCK_SIZE];
  memset(current, 0, BLOCK_SIZE);
  datas.push_back(current);
  pos = 0;
  remain = 8;
}

int getMinimumCodeSize(int numColors) {
  // cannot be smaller than 2
  int size = 2;
  while (numColors > 1 << size) {
    ++size;
  }
  return size;
}

void LzwEncoder::writeBits(uint32_t src, int32_t bitNum) {
  while (0 < bitNum) {
    if (remain <= bitNum) {
      current[pos] = uint8_t(current[pos] | (src << (8 - remain)));
      src >>= remain;
      bitNum -= remain;
      remain = 8;
      ++pos;
      if (pos == BLOCK_SIZE) {
        current = new uint8_t[BLOCK_SIZE];
        memset(current, 0, BLOCK_SIZE);
        datas.push_back(current);
        pos = 0;
      }
    } else {
      current[pos] =
          uint8_t(uint(current[pos] << bitNum) | (((1 << bitNum) - 1) & src));
      remain -= bitNum;
      bitNum = 0;
    }
  }
}

static const uint8_t BLOCK_TERMINATOR = 0x00;
// static const uint8_t BLOCK_MIN_CODE_SIZE = 0x8;

int LzwEncoder::write(std::vector<uint8_t> &content, uint8_t minimumCodeSize) {
  content.push_back(minimumCodeSize);
  int size;
  int total = 0;
  for (auto block : datas) {
    size = block == current ? (remain == 0 ? pos : pos + 1) : BLOCK_SIZE;
    total = total + size;
    content.push_back(uchar(size));
    for (int i = 0; i < size; ++i) {
      content.push_back(block[i]);
    }
  }
  content.push_back(BLOCK_TERMINATOR);
  return total;
}

void LzwEncoder::encode(uint8_t indices[], uint16_t width, uint16_t height,
                        std::vector<uint8_t> &content) {
  uint8_t *endPixels = indices + width * height;
  uint8_t dataSize = 8;
  uint32_t codeSize = dataSize + 1;
  uint32_t codeMask = (1 << codeSize) - 1;

  std::vector<uint16_t> lzwInfoHolder;
  lzwInfoHolder.resize(MAX_STACK_SIZE * BYTE_NUM);
  uint16_t *lzwInfos = &lzwInfoHolder[0];
  uint32_t clearCode = 1 << dataSize;
  uint32_t eolCode = clearCode + 2;
  uint16_t current = *indices;
  indices++;

  writeBits(clearCode, int32_t(codeSize));

  uint16_t *next;
  while (endPixels > indices) {
    next = &lzwInfos[current * BYTE_NUM + *indices];
    if (0 == *next || *next >= MAX_STACK_SIZE) {
      writeBits(current, int32_t(codeSize));
      *next = uint16_t(eolCode);
      if (eolCode < MAX_STACK_SIZE) {
        ++eolCode;
      } else {
        writeBits(clearCode, int32_t(codeSize));
        eolCode = clearCode + 2;
        codeSize = dataSize + 1;
        codeMask = (1 << codeSize) - 1;
        memset(lzwInfos, 0, MAX_STACK_SIZE * BYTE_NUM * sizeof(uint16_t));
      }
      if (codeMask < eolCode - 1 && eolCode < MAX_STACK_SIZE) {
        ++codeSize;
        codeMask = (1 << codeSize) - 1;
      }
      if (endPixels <= indices) {
        break;
      }
      current = *indices;
    } else {
      current = *next;
    }
    indices++;
  }
  writeBits(current, int32_t(codeSize));
  write(content, uint8_t(getMinimumCodeSize(numColors)));
}
