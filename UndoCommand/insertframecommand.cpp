#include "insertframecommand.h"

InsertFrameCommand::InsertFrameCommand(GifHelper *helper, QListWidget *imglist,
                                       int index,
                                       QVector<Magick::Image> &images,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), oldindex(index), oldimgs(images),
      imgw(imglist) {}

void InsertFrameCommand::undo() {
  auto len = oldimgs.count();
  for (auto i = 0; i < len; i++) {
    gif->removeFrame(oldindex);
  }
  imgw->setCurrentRow(oldindex);
}

void InsertFrameCommand::redo() {
  for (auto p = oldimgs.rbegin(); p != oldimgs.rend(); p++) {
    gif->insertNativeImage(*p, oldindex);
  }
  imgw->setCurrentRow(oldindex + oldimgs.count());
}
