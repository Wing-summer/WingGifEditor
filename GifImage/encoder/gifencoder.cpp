#include "gifencoder.h"
#include <QImage>

GifEncoder::GifEncoder(QObject *parent) : QObject(parent) {}

bool GifEncoder::open(QString filename, int width, int height) {
  CGIFrgb_Config config;
  memset(&config, 0, sizeof(config));
  config.path = filename.toLocal8Bit().constData();
  config.width = uint16_t(width);
  config.height = uint16_t(height);
  pGIF = cgif_rgb_newgif(&config);
  return pGIF;
}

bool GifEncoder::push(QImage &image, int delayTime) {
  if (pGIF == nullptr) {
    return false;
  }
  CGIFrgb_FrameConfig fconfig;
  memset(&fconfig, 0, sizeof(fconfig));
  fconfig.pImageData = reinterpret_cast<uint8_t *>(
      const_cast<unsigned char *>(image.constBits()));
  fconfig.fmtChan = CGIF_CHAN_FMT_RGBA;
  fconfig.delay = uint16_t(delayTime);
  cgif_rgb_addframe(pGIF, &fconfig);
  return true;
}

bool GifEncoder::close() {
  if (cgif_rgb_close(pGIF) == CGIF_OK) {
    pGIF = nullptr;
    return true;
  }
  return false;
}
