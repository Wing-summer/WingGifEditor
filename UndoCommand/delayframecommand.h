#ifndef DELAYFRAMECOMMAND_H
#define DELAYFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QUndoCommand>

class DelayFrameCommand : public QUndoCommand {
public:
  DelayFrameCommand(GifDecoder *helper, QVector<int> &indices, int delay,
                    QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  int newdelay;
  QVector<int> olddelay, oldindices;
};

class DelayScaleCommand : public QUndoCommand {
public:
  DelayScaleCommand(GifDecoder *helper, QVector<int> &indices, int scale,
                    QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  QVector<int> olddelay, newdelay, oldindices;
};

#endif // DELAYFRAMECOMMAND_H
