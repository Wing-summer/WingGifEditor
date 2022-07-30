#ifndef REMOVEFRAMECOMMAND_H
#define REMOVEFRAMECOMMAND_H

#include "GifImage/gifhelper.h"
#include <Magick++.h>
#include <QMap>
#include <QUndoCommand>

class RemoveFrameCommand : public QUndoCommand {
public:
  RemoveFrameCommand(GifHelper *helper, QVector<int> &frames,
                     QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

protected:
  GifHelper *gif;
  QVector<int> indices;
  QVector<Magick::Image> imgs;
};

#endif // REMOVEFRAMECOMMAND_H
