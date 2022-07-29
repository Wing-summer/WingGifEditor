#include "removeframecommand.h"

RemoveFrameCommand::RemoveFrameCommand(GifHelper *helper,
                                       QMap<int, Magick::Image> &frames,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper) {
  auto k = frames.keys();
  std::sort(k.begin(), k.end());
  indices.append(k.toVector());
  auto len = k.count();
  imgs.resize(len);
  for (auto i = 0; i < len; i++) {
    imgs[i] = frames[indices[i]];
  }
}

void RemoveFrameCommand::undo() {
  for (auto i : indices)
    gif->insertNativeImage(imgs[i], i);
}

void RemoveFrameCommand::redo() {
  for (auto p = indices.rbegin(); p < indices.rend(); p++)
    gif->removeFrame(*p);
}
