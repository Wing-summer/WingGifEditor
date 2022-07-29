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
  QByteArray framedata(int index);

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
  int mergeGif(QString &gif, int index = -1);
  int mergeGifs(QStringList &gifs, int index = -1);

  bool insertPic(QString &pic, int index = -1);
  int insertPics(QStringList &imgs, int index = -1);

  void reduceFrame(int from, int to, int step);
  void createReverse(int from, int to);

  bool exportImages(QString folder, QString ext);

  bool addFrameData(int index, QByteArray &buffer);
  void scale(int w, int h);
  void crop(int x, int y, int w, int h);

  //+++++++++++++++++++++++++++++

  void insertNativeImage(Magick::Image &img, int index);
  void getNativeImages(QVector<int> &indices, QVector<Magick::Image> &imgs);

signals:
  void frameRemoved(int index);
  void frameRefreshAll();
  void frameMoved(int from, int to);
  void frameDelaySet(int index, int time);
  void frameRefresh(int index);
  void frameMerge(int start, int count);
  void frameInsert(int index);
  void frameImageChanged();

private:
  GifImage m_gif;
  QList<QImage> m_preview;

private:
  void generatePreview();
};

#endif // GIFHELPER_H
