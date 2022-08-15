#include "insertframecommand.h"

InsertFrameCommand::InsertFrameCommand(GifDecoder *helper, QListWidget *imglist,
                                       int index,
                                       QList<QGifFrameInfoData> &images,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), oldindex(index), oldimgs(images),
      imgw(imglist) {}

void InsertFrameCommand::undo() {
  auto len = oldimgs.count();
  auto frames = gif->frames();
  for (auto i = 0; i < len; i++) {
    gif->removeFrame(oldindex);
  }
  gif->frameRefreshLabel(oldindex);
  imgw->clearSelection();
  imgw->setCurrentRow(oldindex);
}

void InsertFrameCommand::redo() {
  for (auto p = oldimgs.rbegin(); p != oldimgs.rend(); p++) {
    gif->insertFrame(oldindex, *p);
  }
  gif->frameRefreshLabel(oldindex);
  imgw->clearSelection();
  imgw->setCurrentRow(oldindex);
}
