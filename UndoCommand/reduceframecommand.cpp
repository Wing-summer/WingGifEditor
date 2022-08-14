#include "reduceframecommand.h"

ReduceFrameCommand::ReduceFrameCommand(GifDecoder *helper,
                                       QVector<int> &indices,
                                       QVector<int> &interval,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), oldindex(indices), inter(interval) {
  auto len = gif->frameCount();
  for (auto i = 0; i < len; i++) {
    oldinter.append(gif->frameDelay(i));
  }
  auto frames = gif->frames();
  for (auto i : indices) {
    oldimgs.append(frames[i]);
  }
}

void ReduceFrameCommand::undo() {
  auto pi = oldindex.rbegin();
  auto pp = oldimgs.rbegin();

  for (; pi != oldindex.rend(); pi++, pp++) {
    gif->insertFrame(*pi, *pp);
  }
  auto len = oldinter.count();
  for (auto i = 0; i < len; i++) {
    gif->setFrameDelay(i, oldinter[i]);
  }
  gif->frameRefreshLabel(0);
}

void ReduceFrameCommand::redo() {
  for (auto p = oldindex.rbegin(); p != oldindex.rend(); p++) {
    gif->removeFrame(*p);
  }
  auto len = inter.count();
  for (auto i = 0; i < len; i++) {
    gif->setFrameDelay(i, inter[i]);
  }
  gif->frameRefreshLabel(0);
}
