#ifndef REMOVEFRAMECOMMAND_H
#define REMOVEFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QMap>
#include <QUndoCommand>

class RemoveFrameCommand : public QUndoCommand {
public:
  RemoveFrameCommand(GifDecoder *helper, QVector<int> &frames,
                     QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

protected:
  GifDecoder *gif;
  QVector<int> indices;
  QVector<QGifFrameInfoData> imgs;
};

#endif // REMOVEFRAMECOMMAND_H
