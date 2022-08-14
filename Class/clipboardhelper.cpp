#include "clipboardhelper.h"
#include <QApplication>
#include <QBuffer>
#include <QClipboard>
#include <QMimeData>
#include <QPainter>

void ClipBoardHelper::setImageFrames(QList<QGifFrameInfoData> &frames) {
  auto clipboard = qApp->clipboard();
  QByteArray buffer;
  auto len = frames.count();
  buffer.append(reinterpret_cast<const char *>(&len), sizeof(int));
  for (auto &frame : frames) {
    QByteArray img;
    QBuffer bu(&img);
    bu.open(QBuffer::WriteOnly);
    frame.image.save(&bu, "PNG");
    len = img.count();
    buffer.append(reinterpret_cast<const char *>(&len), sizeof(int));
    buffer.append(img);
    buffer.append(reinterpret_cast<const char *>(&frame.delayTime),
                  sizeof(int));
    auto tcolor = frame.transparentColor.rgba();
    buffer.append(reinterpret_cast<const char *>(&tcolor), sizeof(uint));
  }
  clipboard->setText(buffer.toHex());
}

void ClipBoardHelper::getImageFrames(QList<QGifFrameInfoData> &frames) {
  frames.clear();
  auto clipboard = qApp->clipboard();
  auto buffer = QByteArray::fromHex(clipboard->text().toUtf8());
  if (buffer.count()) {
    auto pb = buffer.data();
    auto epb = pb + buffer.count();
    int count = *reinterpret_cast<int *>(pb);
    if (count < 0)
      return;
    pb += sizeof(int);
    for (auto i = 0; i < count && pb < epb; i++) {
      auto imgbytes = *reinterpret_cast<int *>(pb);
      pb += sizeof(int);
      QImage img;
      if (img.loadFromData(reinterpret_cast<const uchar *>(pb), imgbytes)) {
        QGifFrameInfoData d;
        QImage im(img.size(), QImage::Format_RGBA8888);
        QPainter p(&im);
        p.drawImage(QPoint(), img);
        d.image.swap(im);
        pb += imgbytes;
        d.delayTime = *reinterpret_cast<int *>(pb);
        pb += sizeof(int);
        d.transparentColor = *reinterpret_cast<QRgb *>(pb);
        pb += sizeof(QRgb);
        frames.append(d);
      } else {
        pb += (ulong(imgbytes) + sizeof(int) + sizeof(QRgb));
      }
    }
  }
}
