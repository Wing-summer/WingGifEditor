#include "removeframecommand.h"

RemoveFrameCommand::RemoveFrameCommand(GifDecoder *helper, QVector<int> &frames,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper) {
  indices = frames;
  std::sort(indices.begin(), indices.end(), std::greater<int>());

  auto fms = helper->frames();
  for (auto i : indices) {
    imgs.append(fms[i]);
  }
}

void RemoveFrameCommand::undo() {
  for (auto i : indices) {
    gif->insertFrame(i, imgs[i]);
  }
  gif->frameRefreshLabel(*indices.end());
}

void RemoveFrameCommand::redo() {
  for (auto p = indices.rbegin(); p < indices.rend(); p++)
    gif->removeFrame(*p);
  gif->frameRefreshLabel(*indices.end());
}
