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
  std::vector<Magick::Image> frames(m_frames.begin(), m_frames.end());
  try {
    GifSaver saver(frames.begin(), frames.end(), filename.toStdString());
    StartWaitFinish(saver);
    return true;
  } catch (...) {
    return false;
  }
}

QImage GifImage::frame(int index) {
  if (index < 0 || index > int(m_frames.size()))
    return QImage();
  return Image2QImage(m_frames[ulong(index)]);
}

int GifImage::frameCount() { return int(m_frames.size()); }

int GifImage::frameDelay(int index) {
  if (index < 0 || index > int(m_frames.size()))
    return -1;
  return int(m_frames[ulong(index)].animationDelay()) * 10;
}

QSize GifImage::size() { return m_size; }

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
