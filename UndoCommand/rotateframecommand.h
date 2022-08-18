#ifndef ROTATEFRAMECOMMAND_H
#define ROTATEFRAMECOMMAND_H

#include "Class/gifeditor.h"
#include "GifImage/decoder/gifdecoder.h"
#include <QUndoCommand>

class RotateFrameCommand : public QUndoCommand {
public:
  RotateFrameCommand(GifDecoder *helper, GifEditor *editor, bool isclockwise,
                     QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  GifEditor *m_editor;
  bool clockwise;
};

#endif // ROTATEFRAMECOMMAND_H
