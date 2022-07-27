#include "gifhelper.h"

GifHelper::GifHelper(QObject *parent) : QObject(parent) {}

bool GifHelper::load(QString filename) {
  auto res = m_gif.load(filename);
  if (!res)
    return false;
  generatePreview();
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

QByteArray GifHelper::framedata(int index) { return m_gif.framedata(index); }

int GifHelper::frameCount() { return m_gif.frameCount(); }

int GifHelper::frameDelay(int index) { return m_gif.frameDelay(index); }

void GifHelper::setFrameDelay(int index, int delay) {
  m_gif.setFrameDelay(index, delay);
  emit frameDelaySet(index, delay);
}

void GifHelper::setAllFrameDelay(int delay) {
  m_gif.setAllFrameDelay(delay);
  auto len = m_gif.frameCount();
  for (int i = 0; i < len; i++) {
    emit frameDelaySet(i, delay);
  }
}

QSize GifHelper::size() { return m_gif.size(); }

void GifHelper::removeFrame(int index) {
  m_gif.removeFrame(index);
  m_preview.removeAt(index);
  emit frameRemoved(index);
}

void GifHelper::reverse() {
  m_gif.reverse();
  generatePreview();
  emit frameRefreshAll();
}

bool GifHelper::moveleft(int index) {
  auto res = m_gif.moveleft(index);

  if (res) {
    m_preview.move(index, index - 1);
    emit frameMoved(index, index - 1);
  }
  return res;
}

bool GifHelper::moveright(int index) {
  auto res = m_gif.moveright(index);
  if (res) {
    m_preview.move(index, index + 1);
    emit frameMoved(index, index + 1);
  }
  return res;
}

void GifHelper::flip(FlipDirection dir) {
  m_gif.flip(dir);
  generatePreview();
  emit frameRefreshAll();
}

void GifHelper::rotate(bool clockwise) {
  m_gif.rotate(clockwise);
  generatePreview();
  emit frameRefreshAll();
}

bool GifHelper::applymodel(QString filename, QVector<int> indices) {
  auto res = m_gif.applymodel(filename, indices);
  if (res) {
    for (auto i : indices) {
      m_preview[i] = m_gif.frame(i);
      emit frameRefresh(i);
    }
  }
  return res;
}

int GifHelper::merge(QString &gif, int index) {
  auto res = m_gif.merge(gif, index);
  auto len = index + res;
  if (res > 0) {
    for (auto i = len; i >= index; i--) {
      m_preview.insert(index, m_gif.frame(i));
    }
    emit frameMerge(index, res);
  }
  return res;
}

void GifHelper::reduceFrame(int from, int to, int step) {
  m_gif.reduceFrame(from, to, step); //输出的删除索引为倒序的
  generatePreview();
  emit frameRefreshAll();
}

void GifHelper::createReverse(int from, int to) {
  m_gif.createReverse(from, to);
  auto len = to - from + 1;
  for (auto i = from; i <= to; i++) {
    m_preview.insert(to + 1, m_gif.frame(i));
  }
  emit frameMerge(to + 1, len);
}

bool GifHelper::exportImages(QString folder, QString ext) {
  return m_gif.exportImages(folder, ext);
}

bool GifHelper::addFrameData(int index, QByteArray &buffer) {
  auto res = m_gif.addFrameData(index, buffer);
  if (res) {
    m_preview.insert(index, m_gif.frame(index));
  }
  return res;
}

void GifHelper::scale(int w, int h) {
  m_gif.scale(w, h);
  generatePreview();
  emit frameImageChanged();
}

void GifHelper::crop(int x, int y, int w, int h) {
  m_gif.crop(x, y, w, h);
  generatePreview();
  emit frameImageChanged();
}

void GifHelper::generatePreview() {
  m_preview.clear();
  auto len = m_gif.frameCount();
  for (auto i = 0; i < len; i++) {
    auto img = m_gif.frame(i);
    m_preview.append(img);
  }
}
