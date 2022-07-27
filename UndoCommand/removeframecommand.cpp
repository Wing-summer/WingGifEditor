#include "removeframecommand.h"

RemoveFrameCommand::RemoveFrameCommand(GifHelper *helper,
                                       QMap<int, Magick::Image> &frames,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), olds(frames) {}

void RemoveFrameCommand::undo() {}

void RemoveFrameCommand::redo() {}
