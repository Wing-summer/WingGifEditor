#ifndef GIFDECODER_H
#define GIFDECODER_H

#include "gif_lib.h"
#include <QIcon>
#include <QImage>
#include <QObject>
#include <QPixmap>
#include <QPoint>

enum class FlipDirection { Horizontal, Vertical };

class QGifFrameInfoData {
public:
  QGifFrameInfoData() : delayTime(-1), interlace(false) {}
  QImage image;
  int delayTime;
  bool interlace;
  QColor transparentColor;
};

class GifDecoder : public QObject {
  Q_OBJECT
public:
  explicit GifDecoder(QObject *parent = nullptr);
  bool load(const QString &fileName);
  QList<QGifFrameInfoData> &frames();
  QSize size();

  static QString GetErrorString(int ErrorCode);

  QIcon thumbnail(int index);
  QPixmap frameimg(int index);
  QImage img(int index);

  int frameCount();
  int frameDelay(int index);

  void loadfromImages(QStringList filenames);
  void loadfromGifs(QStringList gifs);
  bool exportImages(QString folder, QString ext);
  void close();

  void setOnionIndex(int index);
  int getLastError();

  void crop(int x, int y, int w, int h);
  bool moveleft(int index);
  bool moveright(int index);

  void setFrameDelay(int index, int delay);
  void setAllFrameDelay(int delay);
  void removeFrame(int index);
  void insertFrame(int index, QGifFrameInfoData &frame);

  void flip(FlipDirection dir);
  void rotate(bool clockwise);

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
  QVector<QRgb> colorTableFromColorMapObject(ColorMapObject *colorMap,
                                             int transColorIndex = -1) const;
  ColorMapObject *colorTableToColorMapObject(QVector<QRgb> colorTable) const;
  QSize getCanvasSize() const;
  int getFrameTransparentColorIndex(const QGifFrameInfoData &info) const;

private:
  QSize canvasSize;
  int loopCount;
  QColor defaultTransparentColor;

  QVector<QRgb> globalColorTable;
  QColor bgColor;
  QList<QGifFrameInfoData> frameInfos;

  int onionIndex = -1;
  int _lasterr = 0;
};

#endif // GIFDECODER_H
