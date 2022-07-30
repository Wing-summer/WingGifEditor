#include "reduceframecommand.h"

ReduceFrameCommand::ReduceFrameCommand(GifHelper *helper, QVector<int> &indices,
                                       QVector<Magick::Image> &imgs,
                                       QVector<int> &interval,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), oldindex(indices), inter(interval),
      oldimgs(imgs) {
  auto len = gif->frameCount();
  for (auto i = 0; i < len; i++) {
    oldinter.append(gif->frameDelay(i));
  }
}

void ReduceFrameCommand::undo() {
  auto pi = oldindex.rbegin();
  auto pp = oldimgs.rbegin();

  for (; pi != oldindex.rend(); pi++, pp++) {
    gif->insertNativeImage(*pp, *pi);
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
