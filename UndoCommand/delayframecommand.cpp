//#include "delayframecommand.h"

// DelayFrameCommand::DelayFrameCommand(GifHelper *helper, QVector<int>
// &indices,
//                                      int delay, QUndoCommand *parent)
//     : QUndoCommand(parent), gif(helper), newdelay(delay), oldindices(indices)
//     {
//   for (auto item : indices) {
//     olddelay.append(gif->frameDelay(item));
//   }
// }

// void DelayFrameCommand::undo() {
//   auto len = oldindices.count();
//   for (auto i = 0; i < len; i++) {
//     gif->setFrameDelay(oldindices[i], olddelay[i]);
//   }
// }

// void DelayFrameCommand::redo() {
//   if (oldindices.count() == gif->frameCount())
//     gif->setAllFrameDelay(newdelay);
//   else {
//     for (auto item : oldindices) {
//       gif->setFrameDelay(item, newdelay);
//     }
//   }
// }

////=================================================

// DelayScaleCommand::DelayScaleCommand(GifHelper *helper, QVector<int>
// &indices,
//                                      int scale, QUndoCommand *parent)
//     : QUndoCommand(parent), gif(helper), oldindices(indices) {
//   for (auto i : indices) {
//     auto time = gif->frameDelay(i);
//     olddelay.append(time);
//     time = time * scale / 1000;
//     time *= 10;
//     newdelay.append(time);
//   }
// }

// void DelayScaleCommand::undo() {
//   auto len = oldindices.count();
//   for (auto i = 0; i < len; i++) {
//     gif->setFrameDelay(oldindices[i], olddelay[i]);
//   }
// }

// void DelayScaleCommand::redo() {
//   auto len = oldindices.count();
//   for (auto i = 0; i < len; i++) {
//     gif->setFrameDelay(oldindices[i], newdelay[i]);
//   }
// }
