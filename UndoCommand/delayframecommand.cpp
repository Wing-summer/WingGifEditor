#include "delayframecommand.h"

DelayFrameCommand::DelayFrameCommand(GifDecoder *helper, QVector<int> &indices,
                                     int delay, QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), newdelay(delay), oldindices(indices) {
  for (auto item : indices) {
    olddelay.append(gif->frameDelay(item));
  }
}

void DelayFrameCommand::undo() {
  auto len = oldindices.count();
  for (auto i = 0; i < len; i++) {
    gif->setFrameDelay(oldindices[i], olddelay[i]);
  }
}

void DelayFrameCommand::redo() {
  if (oldindices.count())
    for (auto item : oldindices) {
      gif->setFrameDelay(item, newdelay);
    }
  else {
    gif->setAllFrameDelay(newdelay);
  }
}

//=================================================

DelayScaleCommand::DelayScaleCommand(GifDecoder *helper, QVector<int> &indices,
                                     int scale, QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), oldindices(indices) {
  if (indices.count()) {
    for (auto i : indices) {
      auto time = gif->frameDelay(i);
      olddelay.append(time);
      time = time * scale / 1000;
      time *= 10;
      newdelay.append(time);
    }
  } else {
    for (auto &frame : gif->frames()) {
      auto time = frame.delayTime;
      olddelay.append(time);
      time = time * scale / 1000;
      time *= 10;
      newdelay.append(time);
    }
  }
}

void DelayScaleCommand::undo() {
  auto len = oldindices.count();
  if (len) {
    for (auto i = 0; i < len; i++) {
      gif->setFrameDelay(oldindices[i], olddelay[i]);
    }
  } else {
    len = gif->frameCount();
    for (auto i = 0; i < len; i++) {
      gif->setFrameDelay(i, olddelay[i]);
    }
  }
}

void DelayScaleCommand::redo() {
  auto len = oldindices.count();
  if (len) {
    for (auto i = 0; i < len; i++) {
      gif->setFrameDelay(oldindices[i], newdelay[i]);
    }
  } else {
    len = gif->frameCount();
    for (auto i = 0; i < len; i++) {
      gif->setFrameDelay(i, newdelay[i]);
    }
  }
}
