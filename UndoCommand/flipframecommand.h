#ifndef FLIPFRAMECOMMAND_H
#define FLIPFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QUndoCommand>

class FlipFrameCommand : public QUndoCommand {
public:
  FlipFrameCommand(GifDecoder *helper, FlipDirection dir,
                   QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  FlipDirection olddir;
};

#endif // FLIPFRAMECOMMAND_H
