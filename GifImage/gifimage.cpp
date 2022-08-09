#include "gifimage.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
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
    return true;
  } catch (const Magick::Exception &ex) {
    showError(ex.what());
    return false;
  } catch (const std::bad_alloc &ex) {
    showError(ex.what());
    return false;
  } catch (...) {
    showError(tr("Unknown Error!"));
    return false;
  }
}

void GifImage::loadfromImages(QStringList imgs) {
  Magick::Image fir(imgs.first().toStdString());
  auto gifsize = fir.size();
  m_frames.clear();
  fir.animationDelay(4);
  m_frames.push_back(fir);
  for (auto p = imgs.begin() + 1; p != imgs.end(); p++) {
    Magick::Image img((*p).toStdString());
    img.scale(gifsize);
    img.animationDelay(4);
    m_frames.push_back(img);
  }
}

void GifImage::loadfromGifs(QStringList gifs) {
  std::vector<Magick::Image> buffer, cbuffer;
  m_frames.clear();
  try {
    GifReader freader(&buffer, gifs.first().toStdString());
    StartWaitFinish(freader);

    GifCoalescer fcoalescer(&cbuffer, buffer.begin(), buffer.end());
    StartWaitFinish(fcoalescer);
    m_frames.insert(m_frames.end(), cbuffer.begin(), cbuffer.end());
    auto gifsize = cbuffer[0].size();
    for (auto p = gifs.begin() + 1; p != gifs.end(); p++) {
      GifReader reader(&buffer, (*p).toStdString());
      StartWaitFinish(reader);
      GifCoalescer coalescer(&cbuffer, buffer.begin(), buffer.end());
      StartWaitFinish(coalescer);
      for (auto &img : cbuffer) {
        img.scale(gifsize);
      }
      m_frames.insert(m_frames.end(), cbuffer.begin(), cbuffer.end());
    }
  } catch (const Magick::Exception &ex) {
    showError(ex.what());
  } catch (const std::bad_alloc &ex) {
    showError(ex.what());
  } catch (...) {
    showError(tr("Unknown Error!"));
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

void GifImage::close() { m_frames.clear(); }

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

QSize GifImage::size() {
  auto _s = m_frames[0].size();
  return QSize(int(_s.width()), int(_s.height()));
}

QByteArray GifImage::framedata(int index) {
  if (index < 0 || index >= int(m_frames.size()))
    return QByteArray();
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

bool GifImage::getModeledFrames(QString filename, QVector<int> indices,
                                QVector<Magick::Image> &imgs) {
  Magick::Image img(filename.toStdString());
  auto s = img.size();
  if (s != m_frames[0].size())
    return false;
  for (auto i : indices) {
    auto imgb = m_frames[ulong(i)];
    imgb.composite(img, 0, 0, Magick::OverCompositeOp);
    imgs.append(imgb);
  }
  return true;
}

int GifImage::getGifFrames(QString gif, QVector<Magick::Image> &imgs) {
  try {
    std::vector<Magick::Image> img, cimgs;
    GifReader reader(&img, gif.toStdString());
    StartWaitFinish(reader);
    GifCoalescer coalescer(&cimgs, img.begin(), img.end());
    StartWaitFinish(coalescer);
    for (auto item : cimgs) {
      imgs.append(item);
    }
  } catch (...) {
    return 0;
  }
  return imgs.count();
}

void GifImage::getReduceFrame(int from, int to, int step, QVector<int> &indices,
                              QVector<Magick::Image> &imgs,
                              QVector<int> &intervals) {
  auto len = frameCount();
  if (from < 0 || from >= to || to >= len || step >= len - 1)
    return;

  indices.clear();
  imgs.clear();
  intervals.clear();

  auto ii = from + 1;
  auto q = step + 1;
  for (auto i = ii; i <= to; i++) {
    if (i == ii + step) {
      ii += step;
      indices.append(i);
      imgs.append(m_frames[ulong(i)]);
    } else {
      intervals.append(frameDelay(i) * (q + 1) / q);
    }
  }
}

void GifImage::getReverse(int from, int to, QVector<Magick::Image> &imgs) {
  auto len = frameCount();
  if (from < 0 || from >= to || to >= len)
    return;
  imgs.resize(to - from + 1);
  std::reverse_copy(m_frames.begin() + from, m_frames.begin() + to,
                    imgs.begin());
}

bool GifImage::exportImages(QString folder, QString ext) {
  int i = 0;
  if (!QFile::exists(folder))
    return false;
  for (auto &img : m_frames) {
    img.write(QString("%1/%2.%3").arg(folder).arg(i).arg(ext).toStdString());
  }
  return true;
}

bool GifImage::addFrameData(int index, QByteArray &buffer) {
  auto p = reinterpret_cast<Magick::Image *>(buffer.data());
  if (!p || !p->magick().length())
    return false;
  m_frames.insert(m_frames.begin() + index, *p);
  return true;
}

void GifImage::scale(int w, int h) {
  auto newsize = Magick::Geometry(uint(w), uint(h));
  newsize.aspect(true);
  for (auto &img : m_frames) {
    img.resize(newsize);
  }
}

void GifImage::crop(int x, int y, int w, int h) {
  auto newsize = Magick::Geometry(uint(w), uint(h), uint(x), uint(y));
  for (auto &img : m_frames) {
    img.crop(newsize);
  }
}

void GifImage::getNativeFrames(QVector<int> &indices,
                               QVector<Magick::Image> &frames) {
  frames.clear();
  for (auto i : indices) {
    frames.append(m_frames[ulong(i)]);
  }
}

void GifImage::insertNativeImage(Magick::Image &img, int index) {
  m_frames.insert(m_frames.begin() + index, img);
}

void GifImage::getNativeImages(QVector<int> &indices,
                               QVector<Magick::Image> &imgs) {
  imgs.clear();
  auto max = frameCount();
  for (auto i : indices) {
    if (i >= 0 && i < max) {
      imgs.append(m_frames[ulong(i)]);
    } else {
      imgs.append(Magick::Image());
    }
  }
}

void GifImage::getNativeImagesBefore(int index, QVector<Magick::Image> &imgs) {
  imgs.resize(index);
  for (auto i = 0; i < index; i++) {
    imgs[i] = m_frames[ulong(i)];
  }
}

void GifImage::getNativeImagesAfter(int index, QVector<Magick::Image> &imgs) {
  auto len = frameCount();
  imgs.clear();
  for (auto i = index + 1; i < len; i++) {
    imgs.push_back(m_frames[ulong(i)]);
  }
}

void GifImage::applyNativeImage(Magick::Image &img, int index) {
  if (index <= 0 || index > frameCount())
    return;
  m_frames[ulong(index)] = img;
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

void GifImage::showError(QString err) {
  QMessageBox::critical(nullptr, tr("Error"), err);
}
