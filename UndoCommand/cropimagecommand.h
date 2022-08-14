#ifndef CROPIMAGECOMMAND_H
#define CROPIMAGECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QListWidget>
#include <QUndoCommand>

class CropImageCommand : public QUndoCommand {
public:
  CropImageCommand(GifDecoder *helper, int x, int y, int w, int h,
                   QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  QList<QImage> buffer;
  QRect rect;
};

#endif // CROPIMAGECOMMAND_H
