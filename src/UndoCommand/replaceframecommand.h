#ifndef REPLACEFRAMECOMMAND_H
#define REPLACEFRAMECOMMAND_H

#include "GifImage/gifhelper.h"
#include <QUndoCommand>

class ReplaceFrameCommand : public QUndoCommand {
public:
  ReplaceFrameCommand(GifHelper *helper, QVector<int> &indices,
                      QVector<Magick::Image> newimgs,
                      QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifHelper *gif;
  QVector<int> olds;
  QVector<Magick::Image> newimages, oldimages;
};

#endif // REPLACEFRAMECOMMAND_H
