//
// Created by succlz123 on 17-9-19.
//

#ifndef BURSTLINKER_DITHERER_H
#define BURSTLINKER_DITHERER_H

#include "./GifImage/quantizer/ColorQuantizer.h"
#include <QObject>
#include <cstdint>

namespace blk {

class Ditherer : public QObject {
  Q_OBJECT
public:
  // ffmpeg vf_paletteuse.c
  static int bayerDitherValue(int p) {
    const int q = p ^ (p >> 3);
    return (p & 4) >> 2 | (q & 4) >> 1 | (p & 2) << 1 | (q & 2) << 2 |
           (p & 1) << 4 | (q & 1) << 5;
  }

  // only for bayer
  int bayerScale = 1;

  uint16_t width = 0;

  uint16_t height = 0;

  virtual ~Ditherer() = default;

  virtual void dither(std::vector<ARGB> &origin, std::vector<ARGB> &quantize,
                      uint8_t *colorIndices) = 0;
};

} // namespace blk

#endif // BURSTLINKER_DITHERER_H
