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
  void setFrameDelay(int index, int delay);
  void setAllFrameDelay(int delay);

  QSize size();

  void removeFrame(int index);
  void reverse();
  bool moveleft(int index);
  bool moveright(int index);

  void flip(FlipDirection dir);
  void rotate(bool clockwise = true);
  bool applymodel(QString filename, QVector<int> indices);
  int merge(QString &gif, int index = -1);

signals:
  void frameRemoved(int index);
  void frameRefreshAll();
  void frameMoved(int from, int to);
  void frameDelaySet(int index, int time);
  void frameRefresh(int index);
  void frameMerge(int start, int count);

private:
  GifImage m_gif;
  QList<QImage> m_preview;

private:
  void generatePreview();
};

#endif // GIFHELPER_H
