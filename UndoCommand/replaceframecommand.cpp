#include "replaceframecommand.h"

ReplaceFrameCommand::ReplaceFrameCommand(GifDecoder *helper,
                                         QVector<int> &indices,
                                         QVector<QImage> &newimgs,
                                         QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), olds(indices), bufferimage(newimgs) {}

void ReplaceFrameCommand::undo() {
  auto frames = gif->frames();
  auto len = olds.count();
  for (auto i = 0; i < len; i++) {
    auto index = olds[i];
    frames[index].image.swap(bufferimage[i]);
    gif->frameRefreshImg(index);
  }
}

void ReplaceFrameCommand::redo() {
  auto frames = gif->frames();
  auto len = olds.count();
  for (auto i = 0; i < len; i++) {
    auto index = olds[i];
    frames[index].image.swap(bufferimage[i]);
    gif->frameRefreshImg(index);
  }
}
