#include "gifimage.h"
#include <QApplication>
#include <QDebug>
#include <QPixmap>
#include <QThreadPool>

#define StartWaitFinish(run)                                                   \
  QThreadPool::globalInstance()->start(&run);                                  \
  waitThreadPool();                                                            \
  if (run.exception())                                                         \
    std::rethrow_exception(run.exception());

GifImage::GifImage(QObject *parent) : QObject(parent) {
  Magick::InitializeMagick(nullptr);
}

bool GifImage::load(QString filename) {
  try {
    std::vector<Magick::Image> frames;
    GifReader reader(&frames, filename.toStdString());
    StartWaitFinish(reader);

    m_frames.clear();
    GifCoalescer coalescer(&m_frames, frames.begin(), frames.end());
    StartWaitFinish(coalescer);

    auto _s = m_frames[0].size();
    m_size = QSize(int(_s.width()), int(_s.height()));

    return true;
  } catch (const Magick::Exception &ex) {
    qDebug() << ex.what();
    return false;
  } catch (const std::bad_alloc &) {
    return false;
  }
}

bool GifImage::save(QString filename) {
  std::vector<Magick::Image> frames;
  Magick::deconstructImages(&frames, m_frames.begin(), m_frames.end());
  try {
    GifSaver saver(frames.begin(), frames.end(), filename.toStdString());
    StartWaitFinish(saver);
    return true;
  } catch (...) {
    return false;
  }
}

QImage GifImage::frame(int index) {
  if (index < 0 || index >= int(m_frames.size()))
    return QImage();
  return Image2QImage(m_frames[ulong(index)]);
}

int GifImage::frameCount() { return int(m_frames.size()); }

int GifImage::frameDelay(int index) {
  if (index < 0 || index >= int(m_frames.size()))
    return -1;
  return int(m_frames[ulong(index)].animationDelay()) * 10;
}

void GifImage::setFrameDelay(int index, int delay) {
  if (index < 0 || index >= int(m_frames.size()))
    return;
  m_frames[ulong(index)].animationDelay(uint(delay) / 10);
}

void GifImage::setAllFrameDelay(int delay) {
  for (auto &item : m_frames) {
    item.animationDelay(uint(delay) / 10);
  }
}

QSize GifImage::size() { return m_size; }

QByteArray GifImage::framedata(int index) {
  return QByteArray(reinterpret_cast<char *>(&m_frames[ulong(index)]),
                    sizeof(Magick::Image));
}

void GifImage::removeFrame(int index) {
  if (index < 0 || index >= int(m_frames.size()))
    return;
  m_frames.erase(m_frames.begin() + index);
}

bool GifImage::moveleft(int index) {
  if (index < 1 || index >= int(m_frames.size()))
    return false;
  std::swap(m_frames[ulong(index)], m_frames[ulong(index - 1)]);
  return true;
}

bool GifImage::moveright(int index) {
  if (index < 0 || index >= int(m_frames.size()) - 1)
    return false;
  std::swap(m_frames[ulong(index)], m_frames[ulong(index + 1)]);
  return true;
}

void GifImage::reverse() { std::reverse(m_frames.begin(), m_frames.end()); }

void GifImage::flip(FlipDirection dir) {
  switch (dir) {
  case FlipDirection::Horizontal: {
    for (auto &item : m_frames) {
      item.flop();
    }
    break;
  }
  case FlipDirection::Vertical: {
    for (auto &item : m_frames) {
      item.flip();
    }
    break;
  }
  }
}

void GifImage::rotate(bool clockwise) {
  for (auto &item : m_frames) {
    item.rotate(clockwise ? -90 : 90);
  }
}

bool GifImage::applymodel(QString filename, QVector<int> indices) {
  Magick::Image img(filename.toStdString());
  auto s = img.size();
  if (s.width() != uint(m_size.width()) || s.height() != uint(m_size.height()))
    return false;
  for (auto i : indices)
    m_frames[ulong(i)].composite(img, 0, 0, Magick::OverCompositeOp);
  return true;
}

int GifImage::merge(QString gif, int index) {
  if (index >= frameCount())
    return -1;
  std::vector<Magick::Image> imgs;
  Magick::readImages(&imgs, gif.toStdString());
  std::vector<Magick::Image> cimgs;
  Magick::coalesceImages(&cimgs, imgs.begin(), imgs.end());
  m_frames.insert(index > 0 ? (m_frames.begin() + index) : m_frames.end(),
                  cimgs.begin(), cimgs.end());
  return int(cimgs.size());
}

void GifImage::reduceFrame(int from, int to, int step) {
  auto len = frameCount();
  if (from < 0 || from >= to || to >= len || step >= len - 1)
    return;

  for (auto i = from + 1; i <= to; i += step) {
    removeFrame(i);
    to--;
  }

  auto q = uint(step) + 1;
  for (auto &item : m_frames) {
    item.animationDelay(item.animationDelay() * (q + 1) / q);
  }
}

void GifImage::createReverse(int from, int to) {
  auto len = frameCount();
  if (from < 0 || from >= to || to >= len)
    return;
  std::vector<Magick::Image> tmp(ulong(to - from + 1));
  std::reverse_copy(m_frames.begin() + from, m_frames.begin() + to,
                    tmp.begin());
  m_frames.insert(m_frames.end(), tmp.begin(), tmp.end());
}

void GifImage::waitThreadPool() {
  while (!QThreadPool::globalInstance()->waitForDone(100 / 6))
    QApplication::processEvents();
}

QImage GifImage::Image2QImage(const Magick::Image &img) {
  QImage qimg(static_cast<int>(img.columns()), static_cast<int>(img.rows()),
              QImage::Format_RGB888);
  const Magick::PixelPacket *pixels;
  Magick::ColorRGB rgb;

  for (int y = 0; y < qimg.height(); ++y) {
    pixels =
        img.getConstPixels(0, y, static_cast<unsigned int>(qimg.width()), 1);

    for (int x = 0; x < qimg.width(); ++x) {
      rgb = (*(pixels + x));

      qimg.setPixel(x, y,
                    QColor(static_cast<int>(255 * rgb.red()),
                           static_cast<int>(255 * rgb.green()),
                           static_cast<int>(255 * rgb.blue()))
                        .rgb());
    }
  }

  return qimg;
}