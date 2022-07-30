#ifndef REDUCEFRAMECOMMAND_H
#define REDUCEFRAMECOMMAND_H

#include "GifImage/gifhelper.h"
#include <QUndoCommand>

class ReduceFrameCommand : public QUndoCommand {
public:
  ReduceFrameCommand(GifHelper *helper, QVector<int> &indices,
                     QVector<Magick::Image> &imgs, QVector<int> &interval,
                     QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifHelper *gif;
  QVector<int> oldindex, inter, oldinter;
  QVector<Magick::Image> oldimgs;
};

#endif // REDUCEFRAMECOMMAND_H
