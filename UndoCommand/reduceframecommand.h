#ifndef REDUCEFRAMECOMMAND_H
#define REDUCEFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QUndoCommand>

class ReduceFrameCommand : public QUndoCommand {
public:
  ReduceFrameCommand(GifDecoder *helper, QVector<int> &indices,
                     QVector<int> &interval, QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  QVector<int> oldindex, inter, oldinter;
  QVector<QGifFrameInfoData> oldimgs;
};

#endif // REDUCEFRAMECOMMAND_H
