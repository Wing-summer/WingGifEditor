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
  void loadfromImages(QStringList imgs);
  void loadfromGifs(QStringList gifs);
  void close();
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
  bool getModeledFrames(QString filename, QVector<int> indices,
                        QVector<Magick::Image> &frames);

  void getNativeFrames(QVector<int> &indices, QVector<Magick::Image> &frames);
  int getNativeMergeGifFrames(QStringList &gifs,
                              QVector<Magick::Image> &frames);
  int getNativeImages(QStringList &imgfiles, QVector<Magick::Image> &imgs);
  void getReduceFrame(int from, int to, int step, QVector<int> &indices,
                      QVector<Magick::Image> &imgs, QVector<int> &intervals);
  void getReverse(int from, int to, QVector<Magick::Image> &imgs);

  bool exportImages(QString folder, QString ext);

  void scale(int w, int h);
  void crop(int x, int y, int w, int h);

  //+++++++++++++++++++++++++++++

  void insertNativeImage(Magick::Image &img, int index);
  void getNativeImages(QVector<int> &indices, QVector<Magick::Image> &imgs);
  void getNativeImagesBefore(int index, QVector<Magick::Image> &imgs);
  void getNativeImagesAfter(int index, QVector<Magick::Image> &imgs);
  void applyNativeImage(Magick::Image &img, int index);

signals:
  void frameRemoved(int index);
  void frameRefreshAll();
  void frameMoved(int from, int to);
  void frameDelaySet(int index, int time);
  void frameRefreshImg(int index);
  void frameRefreshLabel(int start);
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
