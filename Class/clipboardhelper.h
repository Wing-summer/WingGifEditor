#ifndef CLIPBOARDHELPER_H
#define CLIPBOARDHELPER_H

#include "GifImage/decoder/gifdecoder.h"
#include <QObject>

class ClipBoardHelper : public QObject {
  Q_OBJECT
public:
  static void setImageFrames(QList<QGifFrameInfoData> &frames);
  static void getImageFrames(QList<QGifFrameInfoData> &frames);
};

#endif // CLIPBOARDHELPER_H
