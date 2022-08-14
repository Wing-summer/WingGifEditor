#include "delframedircommand.h"

DelFrameDirCommand::DelFrameDirCommand(GifDecoder *helper, int index,
                                       DelDirection dir, QListWidget *listw,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), oldindex(index), olddir(dir),
      listimg(listw) {
  auto frames = helper->frames();
  switch (olddir) {
  case DelDirection::Before: {
    oldimgs = frames.mid(0, index);
    break;
  }
  case DelDirection::After: {
    oldimgs = frames.mid(index + 1);
    break;
  }
  }
}

void DelFrameDirCommand::undo() {
  auto frames = gif->frames();
  switch (olddir) {
  case DelDirection::Before: {
    for (auto p = oldimgs.rbegin(); p != oldimgs.rend(); p++) {
      frames.prepend(*p);
      gif->frameInsert(0);
    }
    gif->frameRefreshLabel(0);
    listimg->setCurrentRow(oldimgs.count());
    break;
  }
  case DelDirection::After: {
    for (auto item : oldimgs) {
      frames.append(item);
      gif->frameInsert(frames.count() - 1);
    }
    auto pos = frames.count() - 1;
    gif->frameRefreshLabel(pos);
    listimg->setCurrentRow(pos);
    break;
  }
  }
}

void DelFrameDirCommand::redo() {
  auto frames = gif->frames();
  switch (olddir) {
  case DelDirection::Before: {
    for (auto i = 0; i < oldindex; i++) {
      frames.removeFirst();
    }
    gif->frameRefreshLabel(0);
    listimg->setCurrentRow(0);
    break;
  }
  case DelDirection::After: {
    auto len = gif->frameCount() - oldindex - 1;
    for (auto i = oldindex + 1; i < len; i++) {
      frames.removeLast();
    }
    gif->frameRefreshLabel(oldindex);
    listimg->setCurrentRow(oldindex);
    break;
  }
  }
}
