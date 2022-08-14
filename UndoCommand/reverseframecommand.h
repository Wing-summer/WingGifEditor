#ifndef REVERSEFRAMECOMMAND_H
#define REVERSEFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QUndoCommand>

class ReverseFrameCommand : public QUndoCommand {
public:
  ReverseFrameCommand(GifDecoder *helper, QUndoCommand *parent = nullptr);
  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
};

#endif // REVERSEFRAMECOMMAND_H
