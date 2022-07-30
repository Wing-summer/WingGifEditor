#ifndef FLIPFRAMECOMMAND_H
#define FLIPFRAMECOMMAND_H

#include "GifImage/gifhelper.h"
#include <QUndoCommand>

class FlipFrameCommand : public QUndoCommand {
public:
  FlipFrameCommand(GifHelper *helper, FlipDirection dir,
                   QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifHelper *gif;
  FlipDirection olddir;
};

#endif // FLIPFRAMECOMMAND_H
