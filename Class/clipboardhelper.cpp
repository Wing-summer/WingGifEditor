#include "clipboardhelper.h"
#include <Magick++.h>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>

void ClipBoardHelper::setImageFrames(QList<QGifFrameInfoData> &frames) {
  auto clipboard = qApp->clipboard();
  QByteArray buffer;
  auto len = frames.count();
  buffer.append(reinterpret_cast<const char *>(&len), sizeof(int));
  for (auto &frame : frames) {
    auto bits = frame.image.constBits();
    len = int(frame.image.sizeInBytes());
    buffer.append(reinterpret_cast<const char *>(&len), sizeof(int));
    buffer.append(reinterpret_cast<const char *>(bits), len);
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
        d.image.swap(img);
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
