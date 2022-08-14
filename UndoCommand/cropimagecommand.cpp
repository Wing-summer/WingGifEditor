#include "cropimagecommand.h"

CropImageCommand::CropImageCommand(GifDecoder *helper, int x, int y, int w,
                                   int h, QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper) {
  rect.setRect(x, y, w, h);
  helper->dumpImage(buffer);
}

void CropImageCommand::undo() {
  gif->swap(buffer);
  gif->frameImageChanged();
}

void CropImageCommand::redo() {
  gif->crop(rect.x(), rect.y(), rect.width(), rect.height());
}
