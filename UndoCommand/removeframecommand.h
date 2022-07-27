#ifndef REMOVEFRAMECOMMAND_H
#define REMOVEFRAMECOMMAND_H

#include "GifImage/gifhelper.h"
#include <Magick++.h>
#include <QMap>
#include <QUndoCommand>

class RemoveFrameCommand : public QUndoCommand {
public:
  RemoveFrameCommand(GifHelper *helper, QMap<int, Magick::Image> &frames,
                     QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

protected:
  GifHelper *gif;
  QMap<int, Magick::Image> olds;
};

#endif // REMOVEFRAMECOMMAND_H
