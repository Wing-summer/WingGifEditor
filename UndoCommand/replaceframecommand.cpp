#include "replaceframecommand.h"

ReplaceFrameCommand::ReplaceFrameCommand(GifDecoder *helper,
                                         QVector<int> &indices,
                                         QVector<QImage> &newimgs,
                                         QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), olds(indices), bufferimage(newimgs) {}

void ReplaceFrameCommand::undo() {
  auto &frames = gif->frames();
  auto len = olds.count();
  if (len) {
    for (auto i = 0; i < len; i++) {
      auto index = olds[i];
      frames[index].image.swap(bufferimage[i]);
      gif->frameRefreshImg(index);
    }
  } else {
    auto p = frames.begin();
    auto pc = bufferimage.begin();
    for (; p != frames.begin(); p++, pc++) {
      (*p).image.swap(*pc);
    }
  }
}

void ReplaceFrameCommand::redo() {
  auto &frames = gif->frames();
  auto len = olds.count();
  if (len) {
    for (auto i = 0; i < len; i++) {
      auto index = olds[i];
      frames[index].image.swap(bufferimage[i]);
      gif->frameRefreshImg(index);
    }
  } else {
    auto p = frames.begin();
    auto pc = bufferimage.begin();
    for (; p != frames.begin(); p++, pc++) {
      (*p).image.swap(*pc);
    }
  }
}
