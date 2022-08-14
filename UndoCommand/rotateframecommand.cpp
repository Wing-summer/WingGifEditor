#include "rotateframecommand.h"

RotateFrameCommand::RotateFrameCommand(GifDecoder *helper, bool isclockwise,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), clockwise(isclockwise) {}

void RotateFrameCommand::undo() { gif->rotate(!clockwise); }

void RotateFrameCommand::redo() { gif->rotate(clockwise); }
