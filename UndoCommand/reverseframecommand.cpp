#include "reverseframecommand.h"

ReverseFrameCommand::ReverseFrameCommand(GifHelper *helper,
                                         QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper) {}

void ReverseFrameCommand::undo() { gif->reverse(); }

void ReverseFrameCommand::redo() { gif->reverse(); }
