#include "reverseframecommand.h"

ReverseFrameCommand::ReverseFrameCommand(GifDecoder *helper,
                                         QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper) {}

void ReverseFrameCommand::undo() {
  auto frames = gif->frames();
  std::reverse(frames.begin(), frames.end());
  gif->frameRefreshAll();
}

void ReverseFrameCommand::redo() {
  auto frames = gif->frames();
  std::reverse(frames.begin(), frames.end());
  gif->frameRefreshAll();
}
