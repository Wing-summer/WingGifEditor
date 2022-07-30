#include "clipboardhelper.h"
#include <Magick++.h>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>

ClipBoardHelper::ClipBoardHelper(GifHelper *helper, QObject *parent)
    : QObject(parent), m_helper(helper) {}

void ClipBoardHelper::setImageFrames(QVector<int> &selections) {
  auto clipboard = qApp->clipboard();
  QByteArray buffer;
  auto l = selections.count();
  buffer.append(reinterpret_cast<const char *>(&l), sizeof(int));
  std::sort(selections.begin(), selections.end(), std::greater<int>());
  for (auto sel : selections) {
    buffer.append(m_helper->framedata(sel));
  }
  clipboard->setText(buffer.toHex());
}

void ClipBoardHelper::getImageFrames(QVector<Magick::Image> &images) {
  images.clear();
  auto clipboard = qApp->clipboard();
  auto buffer = QByteArray::fromHex(clipboard->text().toUtf8());
  if (buffer.count()) {
    int count = *reinterpret_cast<int *>(buffer.data());
    if (count < 0)
      return;
    auto ncount =
        (buffer.count() - int(sizeof(int))) / int(sizeof(Magick::Image));
    auto len = qMin(count, ncount);
    auto off = int(sizeof(int));
    for (auto i = 0; i < len; i++) {
      auto m = buffer.mid(off, sizeof(Magick::Image));
      auto p = reinterpret_cast<Magick::Image *>(m.data());
      if (!p || !p->magick().length() || !p->magick().compare("GIF"))
        continue;
      images.append(*p);
    }
    return;
  }
  return;
}
