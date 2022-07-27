#include "insertframecommand.h"

InsertFrameCommand::InsertFrameCommand(GifHelper *helper, QVector<int> &index,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), olds(index) {}

void InsertFrameCommand::undo() {}

void InsertFrameCommand::redo() {}
