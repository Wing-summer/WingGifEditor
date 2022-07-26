#ifndef GIFHELPER_H
#define GIFHELPER_H

#include "gifimage.h"
#include <QIcon>
#include <QList>
#include <QObject>

class GifHelper : public QObject {
  Q_OBJECT
public:
  explicit GifHelper(QObject *parent = nullptr);

  bool load(QString filename);
  bool save(QString filename);
  QIcon thumbnail(int index);
  QPixmap frameimg(int index);
  QImage img(int index);

  int frameCount();
  int frameDelay(int index);

  QSize size();

private:
  GifImage m_gif;
  QList<QImage> m_preview;
};

#endif // GIFHELPER_H
