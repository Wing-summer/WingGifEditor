#include "delframedircommand.h"

DelFrameDirCommand::DelFrameDirCommand(GifHelper *helper, int index,
                                       DelDirection dir, QListWidget *listw,
                                       QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), oldindex(index), olddir(dir),
      listimg(listw) {
  switch (olddir) {
  case DelDirection::Before: {
    gif->getNativeImagesBefore(index, oldimgs);
    break;
  }
  case DelDirection::After: {
    gif->getNativeImagesAfter(index, oldimgs);
    break;
  }
  }
}

void DelFrameDirCommand::undo() {
  switch (olddir) {
  case DelDirection::Before: {
    for (auto p = oldimgs.rbegin(); p != oldimgs.rend(); p++) {
      gif->insertNativeImage(*p, 0);
    }
    listimg->setCurrentRow(oldimgs.count());
    break;
  }
  case DelDirection::After: {
    for (auto p = oldimgs.rbegin(); p != oldimgs.rend(); p++) {
      gif->insertNativeImage(*p, oldindex + 1);
    }
    listimg->setCurrentRow(oldimgs.count() + oldindex);
    break;
  }
  }
}

void DelFrameDirCommand::redo() {
  switch (olddir) {
  case DelDirection::Before: {
    for (auto i = 0; i < oldindex; i++) {
      gif->removeFrame(0);
    }
    listimg->setCurrentRow(0);
    break;
  }
  case DelDirection::After: {
    auto len = gif->frameCount();
    for (auto i = oldindex + 1; i < len; i++) {
      gif->removeFrame(oldindex + 1);
    }
    listimg->setCurrentRow(oldindex);
    break;
  }
  }
}
