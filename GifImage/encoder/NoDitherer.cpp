//
// Created by succlz123 on 2017/11/15.
//

#include "NoDitherer.h"
#include "./GifImage/quantizer/KDTree.h"
#include <algorithm>
#include <iostream>

using namespace blk;

void NoDitherer::dither(std::vector<ARGB> &origin, std::vector<ARGB> &quantize,
                        uint8_t *colorIndices) {
  size_t totalSize = width * height;
  size_t size = origin.size();
  size_t quantizeSize = quantize.size();
  KDTree kdTree;
  KDTree::Node rootNode;
  auto end = static_cast<uint8_t>(quantizeSize - 1);
  auto transparentColorIndex = static_cast<uint8_t>(quantizeSize + 1);
  kdTree.createKDTree(&rootNode, quantize, 0, end, 0);
  uint16_t lastR = 256;
  uint16_t lastG = 256;
  uint16_t lastB = 256;
  uint8_t lastIndex = 0;
  ulong count = 0;
  for (ulong i = 0; i < size; ++count) {
    auto rgb = origin[i];
    if (rgb.unTranpsparentIndex == uint(count)) {
      ++i;
      if (!(lastR == rgb.r && lastG == rgb.g && lastB == rgb.b)) {
        lastR = rgb.r;
        lastG = rgb.g;
        lastB = rgb.b;
        kdTree.searchNoBacktracking(&rootNode, rgb.r, rgb.g, rgb.b, -1);
        lastIndex = kdTree.nearest.index;
      }
      colorIndices[count] = lastIndex;
    } else {
      colorIndices[count] = transparentColorIndex;
    }
  }
  if (count < totalSize) {
    for (auto i = count; i < totalSize; ++i) {
      colorIndices[i] = transparentColorIndex;
    }
  }
  kdTree.freeKDTree(&rootNode);
}
