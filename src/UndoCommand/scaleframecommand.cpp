#include "scaleframecommand.h"

ScaleFrameCommand::ScaleFrameCommand(GifHelper *helper, int w, int h,
                                     QListWidget *imglist, QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), listimg(imglist), _w(w), _h(h) {
  gif->save(tmp.fileName());
}

void ScaleFrameCommand::undo() {
  auto p = listimg->currentRow();
  gif->load(tmp.fileName());
  listimg->setCurrentRow(p);
}

void ScaleFrameCommand::redo() { gif->scale(_w, _h); }
