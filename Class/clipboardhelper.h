#ifndef CLIPBOARDHELPER_H
#define CLIPBOARDHELPER_H

#include "GifImage/gifhelper.h"
#include <QObject>

class ClipBoardHelper : public QObject {
  Q_OBJECT
public:
  explicit ClipBoardHelper(GifHelper *helper, QObject *parent = nullptr);
  void setImageFrames(QVector<int> &selections);
  void getImageFrames(QVector<Magick::Image> &images);

private:
  GifHelper *m_helper;
};

#endif // CLIPBOARDHELPER_H
