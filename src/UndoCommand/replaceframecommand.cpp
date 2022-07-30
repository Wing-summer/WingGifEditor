#include "replaceframecommand.h"

ReplaceFrameCommand::ReplaceFrameCommand(GifHelper *helper,
                                         QVector<int> &indices,
                                         QVector<Magick::Image> newimgs,
                                         QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), olds(indices), newimages(newimgs) {
  gif->getNativeFrames(indices, oldimages);
}

void ReplaceFrameCommand::undo() {
  auto len = olds.count();
  for (auto i = 0; i < len; i++) {
    gif->applyNativeImage(oldimages[i], olds[i]);
  }
}

void ReplaceFrameCommand::redo() {
  auto len = olds.count();
  for (auto i = 0; i < len; i++) {
    gif->applyNativeImage(newimages[i], olds[i]);
  }
}
