#ifndef MOVEFRAMECOMMAND_H
#define MOVEFRAMECOMMAND_H

#include "GifImage/gifhelper.h"
#include <QListWidget>
#include <QUndoCommand>

enum class MoveFrameDirection { Left, Right };

class MoveFrameCommand : public QUndoCommand {
public:
  MoveFrameCommand(GifHelper *helper, QListWidget *img,
                   MoveFrameDirection direction, int index,
                   QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

protected:
  GifHelper *gif;
  QListWidget *m_img;
  MoveFrameDirection dir;
  int oldindex;
};

#endif // MOVEFRAMECOMMAND_H
