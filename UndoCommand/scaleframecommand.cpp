#include "scaleframecommand.h"

ScaleFrameCommand::ScaleFrameCommand(GifDecoder *helper, int w, int h,
                                     QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), _w(w), _h(h) {
  helper->dumpImage(buffer);
}

void ScaleFrameCommand::undo() {
  gif->swap(buffer);
  gif->frameImageChanged();
}

void ScaleFrameCommand::redo() { gif->scale(_w, _h); }
