#ifndef MOVEFRAMECOMMAND_H
#define MOVEFRAMECOMMAND_H

#include "GifImage/gifhelper.h"
#include <QUndoCommand>

enum class MoveFrameDirection { Left, Right };

class MoveFrameCommand : public QUndoCommand {
public:
  MoveFrameCommand(GifHelper *helper, MoveFrameDirection direction, int index,
                   QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

protected:
  GifHelper *gif;
  MoveFrameDirection dir;
  int oldindex;
};

#endif // MOVEFRAMECOMMAND_H
