#ifndef CLIPBOARDHELPER_H
#define CLIPBOARDHELPER_H

#include "GifImage/gifhelper.h"
#include <QObject>

class ClipBoardHelper : public QObject {
  Q_OBJECT
public:
  explicit ClipBoardHelper(GifHelper *helper, QObject *parent = nullptr);
  void setImageFrames(QList<int> &selections);
  int getImageFrames(int index);

private:
  GifHelper *m_helper;
};

#endif // CLIPBOARDHELPER_H
