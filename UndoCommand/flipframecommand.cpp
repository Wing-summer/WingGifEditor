#include "flipframecommand.h"

FlipFrameCommand::FlipFrameCommand(GifDecoder *helper, FlipDirection dir,
                                   QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), olddir(dir) {}

void FlipFrameCommand::undo() { gif->flip(olddir); }

void FlipFrameCommand::redo() { gif->flip(olddir); }
