#ifndef SCALEFRAMECOMMAND_H
#define SCALEFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QListWidget>
#include <QUndoCommand>

class ScaleFrameCommand : public QUndoCommand {
public:
  ScaleFrameCommand(GifDecoder *helper, int w, int h,
                    QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  QList<QImage> buffer;
  int _w, _h;
};

#endif // SCALEFRAMECOMMAND_H
