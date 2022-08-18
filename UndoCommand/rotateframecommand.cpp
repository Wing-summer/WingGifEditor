#include "rotateframecommand.h"

RotateFrameCommand::RotateFrameCommand(GifDecoder *helper, GifEditor *editor,
                                       bool isclockwise, QUndoCommand *parent)
    : QUndoCommand(parent), gif(helper), m_editor(editor),
      clockwise(isclockwise) {}

void RotateFrameCommand::undo() {
  gif->rotate(!clockwise);
  m_editor->fitPicEditor();
}

void RotateFrameCommand::redo() {
  gif->rotate(clockwise);
  m_editor->fitPicEditor();
}
