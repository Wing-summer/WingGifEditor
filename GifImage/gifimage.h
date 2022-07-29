#ifndef GIFIMAGE_H
#define GIFIMAGE_H

#include <Magick++.h>
#include <Magick++/Exception.h>
#include <QImage>
#include <QObject>
#include <QRunnable>

namespace {

class RunnableEx : public QRunnable {
public:
  std::exception_ptr exception() const { return m_eptr; }
  ~RunnableEx() noexcept override = default;

protected:
  std::exception_ptr m_eptr;
};

class GifReader final : public RunnableEx {
public:
  GifReader(std::vector<Magick::Image> *container, const std::string &fileName)
      : m_container(container), m_fileName(fileName) {
    setAutoDelete(false);
  }

  void run() override {
    try {
      Magick::readImages(m_container, m_fileName);
    } catch (...) {
      m_eptr = std::current_exception();
    }
  }

private:
  std::vector<Magick::Image> *m_container;
  std::string m_fileName;
};

class GifSaver final : public RunnableEx {
public:
  GifSaver(std::vector<Magick::Image>::iterator first,
           std::vector<Magick::Image>::iterator last,
           const std::string &fileName)
      : m_first(first), m_last(last), m_fileName(fileName) {
    setAutoDelete(false);
  }

  void run() override {
    try {
      Magick::writeImages(m_first, m_last, m_fileName);
    } catch (...) {
      m_eptr = std::current_exception();
    }
  }

private:
  std::vector<Magick::Image>::iterator m_first;
  std::vector<Magick::Image>::iterator m_last;
  std::string m_fileName;
};

class GifCoalescer : public RunnableEx {
public:
  GifCoalescer(std::vector<Magick::Image> *container,
               std::vector<Magick::Image>::iterator first,
               std::vector<Magick::Image>::iterator last)
      : m_container(container), m_first(first), m_last(last) {
    setAutoDelete(false);
  }

  void run() override {
    try {
      Magick::coalesceImages(m_container, m_first, m_last);
    } catch (...) {
      m_eptr = std::current_exception();
    }
  }

private:
  std::vector<Magick::Image> *m_container;
  std::vector<Magick::Image>::iterator m_first;
  std::vector<Magick::Image>::iterator m_last;
};

} // namespace

enum class FlipDirection { Horizontal, Vertical };

class GifImage : public QObject {
  Q_OBJECT
public:
  explicit GifImage(QObject *parent = nullptr);
  bool load(QString filename);
  bool save(QString filename);
  QImage frame(int index);
  int frameCount();
  int frameDelay(int index);
  void setFrameDelay(int index, int delay);
  void setAllFrameDelay(int delay);
  QSize size();

  QByteArray framedata(int index);

  void removeFrame(int index);
  bool moveleft(int index);
  bool moveright(int index);

  void reverse();
  void flip(FlipDirection dir);
  void rotate(bool clockwise = true);
  bool applymodel(QString filename, QVector<int> indices);
  int merge(QString gif, int index = -1);

  bool insertPic(QString &pic, int index = -1);

  void reduceFrame(int from, int to, int step);
  void createReverse(int from, int to);

  bool exportImages(QString folder, QString ext);
  bool addFrameData(int index, QByteArray &buffer);
  void scale(int w, int h);
  void crop(int x, int y, int w, int h);

  //+++++++++++++++++++++++++++++

  void insertNativeImage(Magick::Image &img, int index);
  void getNativeImages(QVector<int> &indices, QVector<Magick::Image> &imgs);

private:
  void waitThreadPool();
  QImage Image2QImage(const Magick::Image &img);

private:
  std::vector<Magick::Image> m_frames;
};

#endif // GIFIMAGE_H
