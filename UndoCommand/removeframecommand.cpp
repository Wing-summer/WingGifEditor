#include "removeframecommand.h"

RemoveFrameCommand::RemoveFrameCommand(GifDecoder *helper, QListWidget *listw,
                                       QVector<int> &frames,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), m_list(listw) {
  indices = frames;
  std::sort(indices.begin(), indices.end(), std::greater<int>());

  auto fms = helper->frames();
  for (auto i : indices) {
    imgs.append(fms[i]);
  }
}

void RemoveFrameCommand::undo() {
  for (auto i : indices) {
    gif->insertFrame(i, imgs[i]);
  }
  auto l = indices.last();
  gif->frameRefreshLabel(l);
  m_list->setCurrentRow(l);
}

void RemoveFrameCommand::redo() {
  for (auto p = indices.rbegin(); p < indices.rend(); p++)
    gif->removeFrame(*p);
  auto l = indices.last();
  gif->frameRefreshLabel(l);
  auto c = gif->frameCount();
  if (l >= c)
    l = c;
  m_list->setCurrentRow(l);
}
