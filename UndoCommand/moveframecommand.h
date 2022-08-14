#ifndef MOVEFRAMECOMMAND_H
#define MOVEFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QListWidget>
#include <QUndoCommand>

enum class MoveFrameDirection { Left, Right };

class MoveFrameCommand : public QUndoCommand {
public:
  MoveFrameCommand(GifDecoder *helper, QListWidget *img,
                   MoveFrameDirection direction, int index,
                   QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

protected:
  GifDecoder *gif;
  QListWidget *m_img;
  MoveFrameDirection dir;
  int oldindex;
};

#endif // MOVEFRAMECOMMAND_H
