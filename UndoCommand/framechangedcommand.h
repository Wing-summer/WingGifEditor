#ifndef FRAMECHANGEDCOMMAND_H
#define FRAMECHANGEDCOMMAND_H

#include "GifImage/gifhelper.h"
#include <QMap>
#include <QUndoCommand>

enum class FrameChangeAllOperation { Scale, Crop };

class FrameChangedAllCommand : public QUndoCommand {
public:
  FrameChangedAllCommand(GifHelper *helper, QVector<Magick::Image> &images,
                         QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifHelper *gif;
  QVector<Magick::Image> olds;
};

enum class FrameChangePartOperation { ModelApply };

class FrameChangedPartCommand : public QUndoCommand {
public:
  FrameChangedPartCommand(GifHelper *helper, QMap<int, Magick::Image> &images,
                          QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifHelper *gif;
  QMap<int, Magick::Image> olds;
};

#endif // FRAMECHANGEDCOMMAND_H
