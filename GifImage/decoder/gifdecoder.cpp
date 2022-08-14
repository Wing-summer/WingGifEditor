#include "gifdecoder.h"
#include <QFile>
#include <QIODevice>
#include <QPainter>

int readFromIODevice(GifFileType *gifFile, GifByteType *data, int maxSize) {
  return int(static_cast<QIODevice *>(gifFile->UserData)
                 ->read(reinterpret_cast<char *>(data), maxSize));
}

GifDecoder::GifDecoder(QObject *parent) : QObject(parent) {}

bool GifDecoder::load(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QIODevice::ReadOnly))
    return false;
  static int interlacedOffset[] = {0, 4, 2,
                                   1}; /* The way Interlaced image should. */
  static int interlacedJumps[] = {8, 8, 4,
                                  2}; /* be read - offsets and jumps... */

  int error;
  QIODevice *device = &file;
  GifFileType *gifFile = DGifOpen(device, readFromIODevice, &error);

  if (!gifFile) {
    _lasterr = error;
    return false;
  }

  if (DGifSlurp(gifFile) == GIF_ERROR) {
    _lasterr = error;
    return false;
  }

  frameInfos.clear(); // fixed by wingsummer

  canvasSize.setWidth(gifFile->SWidth);
  canvasSize.setHeight(gifFile->SHeight);
  if (gifFile->SColorMap) {
    globalColorTable = colorTableFromColorMapObject(gifFile->SColorMap);
    if (gifFile->SBackGroundColor < globalColorTable.size())
      bgColor = QColor(globalColorTable[gifFile->SBackGroundColor]);
  }

  QImage pimg(canvasSize, QImage::Format_RGBA8888);
  QPainter p(&pimg);
  p.setRenderHint(QPainter::RenderHint::HighQualityAntialiasing);
  pimg.fill(Qt::transparent);

  for (int idx = 0; idx < gifFile->ImageCount; ++idx) {
    SavedImage gifImage = gifFile->SavedImages[idx];
    int top = gifImage.ImageDesc.Top;
    int left = gifImage.ImageDesc.Left;
    int width = gifImage.ImageDesc.Width;
    int height = gifImage.ImageDesc.Height;

    QGifFrameInfoData frameInfo;
    GraphicsControlBlock gcb;
    DGifSavedExtensionToGCB(gifFile, idx, &gcb);
    int transColorIndex = gcb.TransparentColor;

    QVector<QRgb> colorTable;
    if (gifImage.ImageDesc.ColorMap)
      colorTable = colorTableFromColorMapObject(gifImage.ImageDesc.ColorMap,
                                                transColorIndex);
    else if (transColorIndex != -1)
      colorTable =
          colorTableFromColorMapObject(gifFile->SColorMap, transColorIndex);
    else
      colorTable = globalColorTable;

    if (transColorIndex != -1)
      frameInfo.transparentColor = colorTable[transColorIndex];
    frameInfo.delayTime = gcb.DelayTime * 10; // convert to milliseconds
    // frameInfo.interlace = gifImage.ImageDesc.Interlace;
    // frameInfo.offset = QPoint(left, top);

    QImage image(width, height, QImage::Format_Indexed8);
    image.setOffset(QPoint(left, top)); // Maybe useful for some users.
    image.setColorTable(colorTable);
    if (transColorIndex != -1)
      image.fill(uint(transColorIndex));
    else if (!globalColorTable.isEmpty())
      image.fill(uint(gifFile->SBackGroundColor)); //! ToDo

    if (gifImage.ImageDesc.Interlace) {
      int line = 0;
      for (int i = 0; i < 4; i++) {
        for (int row = interlacedOffset[i]; row < height;
             row += interlacedJumps[i]) {
          memcpy(image.scanLine(row), gifImage.RasterBits + line * width,
                 size_t(width));
          line++;
        }
      }
    } else {
      for (int row = 0; row < height; row++) {
        memcpy(image.scanLine(row), gifImage.RasterBits + row * width,
               size_t(width));
      }
    }

    // Extract other data for the image.
    if (idx == 0) {
      if (gifImage.ExtensionBlockCount > 2) {
        ExtensionBlock *extBlock = gifImage.ExtensionBlocks;
        if (extBlock->Function == APPLICATION_EXT_FUNC_CODE &&
            extBlock->ByteCount == 8) {
          if (QByteArray(reinterpret_cast<char *>(extBlock->Bytes)) ==
              QByteArray("NETSCAPE2.0")) {
            ExtensionBlock *block = gifImage.ExtensionBlocks + 1;
            if (block->ByteCount == 3) {
              loopCount =
                  uchar(block->Bytes[1]) + uchar((block->Bytes[2]) << 8);
            }
          }
        }
      }
    }

    p.drawImage(QPoint(left, top), image);
    frameInfo.image = pimg;
    frameInfos.append(frameInfo);
  }

  DGifCloseFile(gifFile);
  emit frameRefreshAll();
  return true;
}

QList<QGifFrameInfoData> &GifDecoder::frames() { return frameInfos; }

void GifDecoder::dumpImage(QList<QImage> &list) {
  list.clear();
  for (auto &item : frameInfos) {
    list.append(item.image);
  }
}

void GifDecoder::swap(QList<QImage> &list) {
  if (list.count() != this->frameInfos.count())
    return;

  auto p = list.begin();
  auto pc = frameInfos.begin();

  for (; p != list.end(); p++, pc++) {
    QImage img = p->copy();
    (*pc).image.swap(img);
  }
}

QSize GifDecoder::size() { return canvasSize; }

QString GifDecoder::GetErrorString(int ErrorCode) {
  QString Err;
  switch (ErrorCode) {
  case E_GIF_ERR_OPEN_FAILED:
    Err = tr("Failed to open given file");
    break;
  case E_GIF_ERR_WRITE_FAILED:
    Err = tr("Failed to write to given file");
    break;
  case E_GIF_ERR_HAS_SCRN_DSCR:
    Err = tr("Screen descriptor has already been set");
    break;
  case E_GIF_ERR_HAS_IMAG_DSCR:
    Err = tr("Image descriptor is still active");
    break;
  case E_GIF_ERR_NO_COLOR_MAP:
    Err = tr("Neither global nor local color map");
    break;
  case E_GIF_ERR_DATA_TOO_BIG:
    Err = tr("Number of pixels bigger than width * height");
    break;
  case E_GIF_ERR_NOT_ENOUGH_MEM:
    Err = tr("Failed to allocate required memory");
    break;
  case E_GIF_ERR_DISK_IS_FULL:
    Err = tr("Write failed (disk full?)");
    break;
  case E_GIF_ERR_CLOSE_FAILED:
    Err = tr("Failed to close given file");
    break;
  case E_GIF_ERR_NOT_WRITEABLE:
    Err = tr("Given file was not opened for write");
    break;
  case D_GIF_ERR_OPEN_FAILED:
    Err = tr("Failed to open given file");
    break;
  case D_GIF_ERR_READ_FAILED:
    Err = tr("Failed to read from given file");
    break;
  case D_GIF_ERR_NOT_GIF_FILE:
    Err = tr("Data is not in GIF format");
    break;
  case D_GIF_ERR_NO_SCRN_DSCR:
    Err = tr("No screen descriptor detected");
    break;
  case D_GIF_ERR_NO_IMAG_DSCR:
    Err = tr("No Image Descriptor detected");
    break;
  case D_GIF_ERR_NO_COLOR_MAP:
    Err = tr("Neither global nor local color map");
    break;
  case D_GIF_ERR_WRONG_RECORD:
    Err = tr("Wrong record type detected");
    break;
  case D_GIF_ERR_DATA_TOO_BIG:
    Err = tr("Number of pixels bigger than width * height");
    break;
  case D_GIF_ERR_NOT_ENOUGH_MEM:
    Err = tr("Failed to allocate required memory");
    break;
  case D_GIF_ERR_CLOSE_FAILED:
    Err = tr("Failed to close given file");
    break;
  case D_GIF_ERR_NOT_READABLE:
    Err = tr("Given file was not opened for read");
    break;
  case D_GIF_ERR_IMAGE_DEFECT:
    Err = tr("Image is defective, decoding aborted");
    break;
  case D_GIF_ERR_EOF_TOO_SOON:
    Err = tr("Image EOF detected before image complete");
    break;
  default:
    break;
  }
  return Err;
}

QIcon GifDecoder::thumbnail(int index) { return QIcon(frameimg(index)); }

QPixmap GifDecoder::frameimg(int index) {
  auto img = this->img(index);
  if (onionIndex >= 0) {
    QImage p(img.size(), QImage::Format_RGBA8888);
    p.fill(Qt::transparent);
    QPainter painter(&p);
    painter.drawImage(QPointF(), img, img.rect());
    painter.setOpacity(0.6);
    auto d = this->img(onionIndex);
    painter.setCompositionMode(
        QPainter::CompositionMode::CompositionMode_Source);
    painter.drawImage(QPointF(), d, d.rect());
    painter.end();
    return QPixmap::fromImage(p);
  }
  return QPixmap::fromImage(img);
}

QImage GifDecoder::img(int index) {
  if (index < 0 || index >= frameInfos.count()) {
    return QImage();
  }
  return frameInfos[index].image;
}

int GifDecoder::frameCount() { return frameInfos.count(); }

int GifDecoder::frameDelay(int index) {
  if (index < 0 || index >= frameInfos.count()) {
    return -1;
  }

  return frameInfos[index].delayTime;
}

void GifDecoder::loadfromImages(QStringList filenames) {
  frameInfos.clear();
  QImage img;
  while (filenames.count()) {
    if (img.load(filenames.first())) {
      filenames.removeFirst();
      break;
    }
    filenames.removeFirst();
  }
  auto oimg = img;
  auto osize = oimg.size();
  filenames.removeFirst();
  for (auto f : filenames) {
    if (img.load(f)) {
      QGifFrameInfoData frame;
      frame.image = img.scaled(osize);
      frame.transparentColor = Qt::transparent;
      frame.delayTime = 40; // 40 ms
      frameInfos.append(frame);
    }
  }
  emit frameRefreshAll();
}

void GifDecoder::loadfromGifs(QStringList gifs) {
  GifDecoder dec;
  dec.load(gifs.first());
  auto osize = dec.size();
  this->frameInfos.swap(dec.frameInfos);
  gifs.removeFirst();
  for (auto gif : gifs) {
    if (dec.load(gif)) {
      for (auto &item : dec.frameInfos) {
        item.image = item.image.scaled(osize);
        this->frameInfos.append(item);
      }
    }
  }
}

bool GifDecoder::exportImages(QString folder, QString ext) {
  int index = 0;
  for (auto &frame : frameInfos) {
    auto res = frame.image.save(
        QString("%1/winggif-%2.%3").arg(folder).arg(index++).arg(ext));
    if (!res)
      return false;
  }
  return true;
}

void GifDecoder::close() {
  frameInfos.clear();
  canvasSize = QSize();
  emit frameRefreshAll();
}

void GifDecoder::setOnionIndex(int index) {
  if (index >= frameInfos.count()) {
    return;
  }
  onionIndex = index;
  emit frameRefreshImgCurrent();
}

int GifDecoder::getLastError() { return _lasterr; }

void GifDecoder::crop(int x, int y, int w, int h) {
  for (auto &frame : frameInfos) {
    auto &img = frame.image;
    img = img.copy(x, y, w, h);
  }
}

bool GifDecoder::moveleft(int index) {
  if (index > 0 && index < frameInfos.count()) {
    frameInfos.move(index, index - 1);
    emit frameMoved(index, index - 1);
    return true;
  }
  return false;
}

bool GifDecoder::moveright(int index) {
  if (index >= 0 && index < frameInfos.count() - 1) {
    frameInfos.move(index, index + 1);
    emit frameMoved(index, index + 1);
    return true;
  }
  return false;
}

void GifDecoder::setFrameDelay(int index, int delay) {
  if (index < 0 || index >= frameInfos.count())
    return;
  frameInfos[index].delayTime = delay;
  emit frameDelaySet(index, delay);
}

void GifDecoder::setAllFrameDelay(int delay) {
  for (auto &frame : frameInfos) {
    frame.delayTime = delay;
  }
}

void GifDecoder::removeFrame(int index) {
  if (index < 0 || index >= int(frameInfos.size()))
    return;
  frameInfos.removeAt(index);
  emit frameRemoved(index);
}

void GifDecoder::insertFrame(int index, QGifFrameInfoData &frame) {
  if (index < 0 || index > int(frameInfos.size()))
    return;
  frameInfos.insert(index, frame);
  emit frameInsert(index);
}

void GifDecoder::flip(FlipDirection dir) {
  switch (dir) {
  case FlipDirection::Horizontal: {
    for (auto &item : frameInfos) {
      auto &img = item.image;
      img = img.mirrored(true, false);
    }
    break;
  }
  case FlipDirection::Vertical: {
    for (auto &item : frameInfos) {
      auto &img = item.image;
      img = img.mirrored();
    }
    break;
  }
  }
  emit frameImageChanged();
}

void GifDecoder::rotate(bool clockwise) {
  QTransform trans;
  trans.rotate(clockwise ? -90 : 90);
  for (auto &frame : frameInfos) {
    auto &img = frame.image;
    img = img.transformed(trans, Qt::SmoothTransformation);
  }
  emit frameImageChanged();
}

void GifDecoder::scale(int w, int h) {
  for (auto &frame : frameInfos) {
    auto &img = frame.image;
    img = img.scaled(w, h);
  }
  emit frameImageChanged();
}

QVector<QRgb>
GifDecoder::colorTableFromColorMapObject(ColorMapObject *colorMap,
                                         int transColorIndex) const {
  QVector<QRgb> colorTable;
  if (colorMap) {
    for (int idx = 0; idx < colorMap->ColorCount; ++idx) {
      GifColorType gifColor = colorMap->Colors[idx];
      QRgb color = qRgba(gifColor.Red, gifColor.Green, gifColor.Blue,
                         idx != transColorIndex ? 0xFF : 0);
      // For non-transparent color, set the alpha to opaque.
      // modified by wingsummer
      colorTable.append(color);
    }
  }
  return colorTable;
}

ColorMapObject *
GifDecoder::colorTableToColorMapObject(QVector<QRgb> colorTable) const {
  if (colorTable.isEmpty())
    return nullptr;

  ColorMapObject *cmap =
      reinterpret_cast<ColorMapObject *>(malloc(sizeof(ColorMapObject)));
  // num of colors must be a power of 2
  int numColors = 1 << GifBitSize(colorTable.size());
  cmap->ColorCount = numColors;
  // Maybe a bug of giflib, BitsPerPixel is used as size of the color table
  // size.
  cmap->BitsPerPixel = GifBitSize(colorTable.size()); // Todo!
  cmap->SortFlag = false;

  GifColorType *colorValues = new GifColorType[ulong(numColors)];
  for (int idx = 0; idx < colorTable.size(); ++idx) {
    colorValues[idx].Red = uchar(qRed(colorTable[idx]));
    colorValues[idx].Green = uchar(qGreen(colorTable[idx]));
    colorValues[idx].Blue = uchar(qBlue(colorTable[idx]));
  }

  cmap->Colors = colorValues;

  return cmap;
}
