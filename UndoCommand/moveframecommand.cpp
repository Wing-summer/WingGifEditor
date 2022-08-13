//#include "moveframecommand.h"

// MoveFrameCommand::MoveFrameCommand(GifHelper *helper, QListWidget *img,
//                                    MoveFrameDirection direction, int index,
//                                    QUndoCommand *parent)
//     : QUndoCommand(parent), gif(helper), m_img(img), dir(direction),
//       oldindex(index) {}

// void MoveFrameCommand::undo() {
//   switch (dir) {
//   case MoveFrameDirection::Left: {
//     if (gif->moveright(oldindex - 1))
//       m_img->setCurrentRow(oldindex);
//     break;
//   }
//   case MoveFrameDirection::Right: {
//     if (gif->moveleft(oldindex + 1))
//       m_img->setCurrentRow(oldindex);
//     break;
//   }
//   }
// }

// void MoveFrameCommand::redo() {
//   switch (dir) {
//   case MoveFrameDirection::Left: {
//     if (gif->moveleft(oldindex)) {
//       m_img->setCurrentRow(oldindex - 1);
//     }
//     break;
//   }
//   case MoveFrameDirection::Right: {
//     gif->moveright(oldindex);
//     m_img->setCurrentRow(oldindex + 1);
//     break;
//   }
//   }
// }
