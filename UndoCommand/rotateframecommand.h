#ifndef ROTATEFRAMECOMMAND_H
#define ROTATEFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QUndoCommand>

class RotateFrameCommand : public QUndoCommand {
public:
  RotateFrameCommand(GifDecoder *helper, bool isclockwise,
                     QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  bool clockwise;
};

#endif // ROTATEFRAMECOMMAND_H
