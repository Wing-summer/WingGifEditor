#include "gifhelper.h"

GifHelper::GifHelper(QObject *parent) : QObject(parent) {}

bool GifHelper::load(QString filename) {
  auto res = m_gif.load(filename);
  if (!res)
    return false;

  //缓存图片
  auto len = m_gif.frameCount();
  for (auto i = 0; i < len; i++) {
    auto img = m_gif.frame(i);
    m_preview.append(img);
  }
  return true;
}

bool GifHelper::save(QString filename) { return m_gif.save(filename); }

QIcon GifHelper::thumbnail(int index) { return QIcon(frameimg(index)); }

QPixmap GifHelper::frameimg(int index) {
  return QPixmap::fromImage(img(index));
}

QImage GifHelper::img(int index) {
  if (index < 0 || index >= m_preview.count())
    return QImage();
  return m_preview[index];
}

int GifHelper::frameCount() { return m_gif.frameCount(); }

int GifHelper::frameDelay(int index) { return m_gif.frameDelay(index); }

QSize GifHelper::size() { return m_gif.size(); }
