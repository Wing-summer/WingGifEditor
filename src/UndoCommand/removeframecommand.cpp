#include "removeframecommand.h"

RemoveFrameCommand::RemoveFrameCommand(GifHelper *helper, QVector<int> &frames,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper) {
  indices = frames;
  std::sort(indices.begin(), indices.end(), std::greater<int>());
  gif->getNativeImages(indices, imgs);
}

void RemoveFrameCommand::undo() {
  for (auto i : indices)
    gif->insertNativeImage(imgs[i], i);
  gif->frameRefreshLabel(*indices.end());
}

void RemoveFrameCommand::redo() {
  for (auto p = indices.rbegin(); p < indices.rend(); p++)
    gif->removeFrame(*p);
  gif->frameRefreshLabel(*indices.end());
}
