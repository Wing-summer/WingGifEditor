#include "moveframecommand.h"

MoveFrameCommand::MoveFrameCommand(GifHelper *helper,
                                   MoveFrameDirection direction, int index,
                                   QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), dir(direction), oldindex(index) {}

void MoveFrameCommand::undo() {
  switch (dir) {
  case MoveFrameDirection::Left: {
    gif->moveright(oldindex - 1);
    break;
  }
  case MoveFrameDirection::Right: {
    gif->moveleft(oldindex + 1);
    break;
  }
  }
}

void MoveFrameCommand::redo() {
  switch (dir) {
  case MoveFrameDirection::Left: {
    gif->moveleft(oldindex);
    break;
  }
  case MoveFrameDirection::Right: {
    gif->moveright(oldindex);
    break;
  }
  }
}
