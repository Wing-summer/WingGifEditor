//
// Created by succlz123 on 17-9-5.
//

#include "GifEncoder.h"
#include "./GifImage/ThreadPool.h"
#include "./GifImage/quantizer/ColorQuantizer.h"
#include "./GifImage/quantizer/KMeansQuantizer.h"
#include "./GifImage/quantizer/MedianCutQuantizer.h"
#include "./GifImage/quantizer/NeuQuantQuantizer.h"
#include "./GifImage/quantizer/OctreeQuantizer.h"
#include "./GifImage/quantizer/RandomQuantizer.h"
#include "./GifImage/quantizer/UniformQuantizer.h"
#include "BayerDitherer.h"
#include "FloydSteinbergDitherer.h"
#include "GifBlockWriter.h"
#include "LzwEncoder.h"
#include "M2Ditherer.h"
#include "NoDitherer.h"
#include <QDebug>
#include <QPoint>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>

using namespace blk;

static int getColorTableSizeField(int actualTableSize) {
  int size = 0;
  while (1 << (size + 1) < actualTableSize) {
    ++size;
  }
  return size;
}

GifEncoder::~GifEncoder() {
  screenWidth = 0;
  screenHeight = 0;
  outfile.close();
  delete[] rsCacheDir;
}

bool GifEncoder::init(const char *path, uint16_t width, uint16_t height,
                      uint32_t loopCount, uint32_t threadCount) {
  outfile.open(path, std::ios::out | std::ios::binary);
  if (!outfile.is_open()) {
    return false;
  }
  this->screenWidth = width;
  this->screenHeight = height;
  GifBlockWriter::writeHeaderBlock(outfile);
  GifBlockWriter::writeLogicalScreenDescriptorBlock(
      outfile, screenWidth, screenHeight, false, 1, false, 0, 0, 0);
  GifBlockWriter::writeNetscapeLoopingExtensionBlock(outfile, loopCount);
  if (threadCount > 8) {
    threadCount = 8;
  }
  if (threadCount >= 1) {
    threadPool = std::make_unique<ThreadPool>(threadCount);
  }

  qDebug() << "Image size is " << width * height << "\n";
  return true;
}

void GifEncoder::addImages(const std::vector<std::vector<uint32_t>> &images,
                           std::vector<uint32_t> &delay,
                           std::vector<QRect> &imgRect, QuantizerType qType,
                           DitherType dType, int32_t transparencyOption) {
  size_t size = images.size();
  std::vector<std::future<std::vector<uint8_t>>> tasks;
  for (size_t k = 0; k < size; ++k) {
    auto result = threadPool->enqueue([=, &images]() {
      std::vector<uint8_t> content;
      auto &image = images[k];
      addImage(image, delay[k], qType, dType, transparencyOption, imgRect[k],
               content);
      return content;
    });
    tasks.emplace_back(std::move(result));
  }
  for (auto &task : tasks) {
    std::vector<uint8_t> result = task.get();
    flush(result);
  }
}

std::vector<uint8_t> GifEncoder::addImage(const std::vector<uint32_t> &original,
                                          uint32_t delay, QuantizerType qType,
                                          DitherType dType,
                                          int32_t transparencyOption,
                                          QRect imgRect,
                                          std::vector<uint8_t> &content) {
  qDebug() << "Get image pixel " << original.size() << "\n";
  uint32_t size = screenWidth * screenHeight;
  std::unique_ptr<ColorQuantizer> colorQuantizer;
  QString quantizerStr;
  switch (qType) {
  case QuantizerType::Uniform:
    colorQuantizer = std::make_unique<UniformQuantizer>();
    quantizerStr = "UniformQuantizer";
    break;
  case QuantizerType::MedianCut:
    colorQuantizer = std::make_unique<MedianCutQuantizer>();
    quantizerStr = "MedianCutQuantizer";
    break;
  case QuantizerType::KMeans:
    colorQuantizer = std::make_unique<KMeansQuantizer>();
    quantizerStr = "KMeansQuantizer";
    break;
  case QuantizerType::Random:
    colorQuantizer = std::make_unique<RandomQuantizer>();
    quantizerStr = "RandomQuantizer";
    break;
  case QuantizerType::Octree:
    colorQuantizer = std::make_unique<OctreeQuantizer>();
    quantizerStr = "OctreeQuantizer";
    break;
  case QuantizerType::NeuQuant:
    colorQuantizer = std::make_unique<NeuQuantQuantizer>();
    quantizerStr = "NeuQuantQuantizer";
    break;
  }

  std::vector<ARGB> quantizeIn;
  quantizeIn.reserve(size);
  bool enableTransparentColor = ((transparencyOption & 0xff) == 1);
  bool ignoreTranslucency = (((transparencyOption >> 8) & 0xff) == 1);
  bool hasTransparentColor = false;
  uint8_t a = 255;
  for (uint32_t i = 0; i < size; i++) {
    auto color = original[i];
    if (enableTransparentColor) {
      a = static_cast<uint8_t>((color >> 24) & 0xff);
      if (!hasTransparentColor) {
        if ((ignoreTranslucency && a != 255) ||
            (!ignoreTranslucency && a == 0)) {
          hasTransparentColor = true;
        }
      }
    }
    auto b = static_cast<uint8_t>((color >> 16) & 0xff);
    auto g = static_cast<uint8_t>((color >> 8) & 0xff);
    auto r = static_cast<uint8_t>(color & 0xff);
    if (a == 255 || (!ignoreTranslucency && a != 0)) {
      quantizeIn.emplace_back(a, r, g, b, i);
    }
  }

  std::vector<ARGB> quantizeOut;
  quantizeOut.reserve(256);
  int quantizeSize = 0;
  if (size > 256) {
    quantizeSize = colorQuantizer->quantize(
        quantizeIn, hasTransparentColor ? 255 : 256, quantizeOut);
  } else {
    quantizeSize = int(size);
    quantizeOut.assign(quantizeIn.begin(), quantizeIn.end());
  }
  qDebug() << quantizerStr << " size is " << quantizeSize;

  if (quantizeSize <= 0) {
    return content;
  }

  uint8_t transparentColorR = 0;
  uint8_t transparentColorG = 0;
  uint8_t transparentColorB = 0;
  if (hasTransparentColor) {
    std::mt19937 generator(uint32_t(time(nullptr)));
    std::uniform_int_distribution<uint32_t> sizeDis(0, size);
    std::uniform_int_distribution<uint32_t> rgbDis(0, 255);
    int tryCount = 0;
    while (tryCount < 12) {
      uint32_t random = sizeDis(generator);
      if (tryCount >= 6) {
        transparentColorR = static_cast<uint8_t>(rgbDis(generator));
        transparentColorG = static_cast<uint8_t>(rgbDis(generator));
        transparentColorB = static_cast<uint8_t>(rgbDis(generator));
      } else {
        transparentColorR = quantizeIn[random].r;
        transparentColorG = quantizeIn[random].g;
        transparentColorB = quantizeIn[random].b;
      }
      int repeatCount = 0;
      for (int i = 0; i < quantizeSize; i++) {
        auto qColor = quantizeOut[ulong(i)];
        if (transparentColorR == qColor.r && transparentColorG == qColor.g &&
            transparentColorB == qColor.b) {
          break;
        } else {
          repeatCount++;
        }
      }
      if (repeatCount == quantizeSize) {
        break;
      }
      tryCount++;
    }
    if (dType == DitherType::FloydSteinberg) {
      dType = DitherType::Bayer;
    }
  }

  //    int32_t paddedColorCount = GifBlockWriter::paddedSize(quantizeSize);
  int32_t paddedColorCount = 256;
  auto transparentColorIndex = static_cast<int32_t>(quantizeSize + 1);
  GifBlockWriter::writeGraphicsControlExtensionBlock(
      content, 2, false, hasTransparentColor, delay / 10,
      hasTransparentColor ? transparentColorIndex : 0);
  GifBlockWriter::writeImageDescriptorBlock(
      content, uint16_t(imgRect.left()), uint16_t(imgRect.top()),
      uint16_t(imgRect.width()), uint16_t(imgRect.height()), true, false, false,
      getColorTableSizeField(paddedColorCount));
  GifBlockWriter::writeColorTableEntity(content, quantizeOut, paddedColorCount);

  std::unique_ptr<Ditherer> ditherer;
  std::string dithererStr;

  switch (dType) {
  case DitherType::No:
    ditherer = std::make_unique<NoDitherer>();
    dithererStr = "NoDitherer";
    break;
  case DitherType::M2:
    ditherer = std::make_unique<M2Ditherer>();
    dithererStr = "M2Ditherer";
    break;
  case DitherType::Bayer:
    ditherer = std::make_unique<BayerDitherer>();
    dithererStr = "BayerDitherer";
    break;
  case DitherType::FloydSteinberg:
    ditherer = std::make_unique<FloydSteinbergDitherer>();
    dithererStr = "FloydSteinbergDitherer";
    break;
  }

  ditherer->width = screenWidth;
  ditherer->height = screenHeight;
  auto colorIndices = new uint8_t[size];

  if (qType == QuantizerType::Octree && dType == DitherType::No &&
      !hasTransparentColor) {
    static_cast<OctreeQuantizer *>(colorQuantizer.get())
        ->getColorIndices(quantizeIn, colorIndices);
  } else {
    ditherer->dither(quantizeIn, quantizeOut, colorIndices);
  }

  if (hasTransparentColor) {
    GifBlockWriter::writeColorTableTransparency(
        content, transparentColorR, transparentColorG, transparentColorB);
    GifBlockWriter::writeColorTableUnpadded(content, transparentColorIndex,
                                            paddedColorCount);
  } else {
    GifBlockWriter::writeColorTableUnpadded(content, quantizeSize,
                                            paddedColorCount);
  }

  qDebug() << dithererStr.c_str();

  LzwEncoder lzwEncoder(paddedColorCount);
  lzwEncoder.encode(colorIndices, screenWidth, screenHeight, content);
  delete[] colorIndices;
  qDebug() << "LZW encode";
  return content;
}

void GifEncoder::flush(const std::vector<uint8_t> &content) {
  size_t size = content.size();
  for (size_t i = 0; i < size; ++i) {
    outfile.write(reinterpret_cast<const char *>(&content[i]), 1);
  }
}

void GifEncoder::finishEncoding() {
  GifBlockWriter::writeTerminator(outfile);
  outfile.close();
}
