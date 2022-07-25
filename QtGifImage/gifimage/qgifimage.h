/****************************************************************************
** Copyright (c) 2013 Debao Zhang <hello@debao.me>
** All right reserved.
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/
#ifndef QGIFIMAGE_H
#define QGIFIMAGE_H

#include "qgifglobal.h"
#include <QColor>
#include <QImage>
#include <QList>
#include <QVector>

enum class FlipDirection { Horizontal, Vertical };

class QGifImagePrivate;
class Q_GIFIMAGE_EXPORT QGifImage {
  Q_DECLARE_PRIVATE(QGifImage)
public:
  QGifImage();
  QGifImage(const QString &fileName);
  QGifImage(const QSize &size);
  ~QGifImage();

  QVector<QRgb> globalColorTable() const;
  QColor backgroundColor() const;
  void setGlobalColorTable(const QVector<QRgb> &colors,
                           const QColor &bgColor = QColor());
  int defaultDelay() const;
  void setDefaultDelay(int internal);
  QColor defaultTransparentColor() const;
  void setDefaultTransparentColor(const QColor &color);

  int loopCount() const;
  void setLoopCount(int loop);

  int frameCount() const;
  QImage frame(int index) const;

  void addFrame(const QImage &frame, int delay = -1);
  // void addFrame(const QImage &frame, const QPoint &offset, int delay = -1);
  void insertFrame(int index, const QImage &frame, int delay = -1);
  // void insertFrame(int index, const QImage &frame, const QPoint &offset,
  // int delay = -1);

  // QPoint frameOffset(int index) const;
  // void setFrameOffset(int index, const QPoint &offset);
  int frameDelay(int index) const;
  void setFrameDelay(int index, int delay);
  QColor frameTransparentColor(int index) const;
  void setFrameTransparentColor(int index, const QColor &color);

  bool load(QIODevice *device);
  bool load(const QString &fileName);
  bool save(QIODevice *device) const;
  bool save(const QString &fileName,
            Qt::ImageConversionFlags flags = Qt::AutoColor) const;

  //================================
  // extended by wingsummer

public:
  int getLastError();                 //获取最后一个错误号
  void removeFrame(int index);        //删除帧
  void reverse();                     //反转帧
  void flip(FlipDirection dir);       //反转帧图像
  void rotate(bool clockwise = true); //旋转90度图像，默认为顺时针
  int width();
  int height();
  QSize size();
  QPixmap frameimg(int index) const;
  bool moveleft(int index);
  bool moveright(int index);
  void createReverse(int from, int to);
  void applymodel(QImage &model, int index);
  void setAllFrameDelay(int delay);
  void scaleAllFrameDelay(int percent);
  bool merge(QString gif);  //合并 GIF
  void scale(int w, int h); //缩放 GIF

private:
  int _lasterr;
  //================================

private:
  QGifImagePrivate *const d_ptr;
};

#endif // QGIFIMAGE_H
