#ifndef REPLACEFRAMECOMMAND_H
#define REPLACEFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QUndoCommand>

class ReplaceFrameCommand : public QUndoCommand {
public:
  ReplaceFrameCommand(GifDecoder *helper, QVector<int> &indices,
                      QVector<QImage> &newimgs, QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  QVector<int> olds;
  QVector<QImage> bufferimage;
};

#endif // REPLACEFRAMECOMMAND_H
