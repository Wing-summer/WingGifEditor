#include "framechangedcommand.h"

FrameChangedAllCommand::FrameChangedAllCommand(GifHelper *helper,
                                               QVector<Magick::Image> &images,
                                               QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), olds(images) {}

void FrameChangedAllCommand::undo() {}

void FrameChangedAllCommand::redo() {}

//=======================================================

FrameChangedPartCommand::FrameChangedPartCommand(
    GifHelper *helper, QMap<int, Magick::Image> &images, QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), olds(images) {}

void FrameChangedPartCommand::undo() {}

void FrameChangedPartCommand::redo() {}
