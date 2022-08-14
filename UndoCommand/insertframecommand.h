#ifndef INSERTFRAMECOMMAND_H
#define INSERTFRAMECOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QListWidget>
#include <QUndoCommand>

class InsertFrameCommand : public QUndoCommand {
public:
  InsertFrameCommand(GifDecoder *helper, QListWidget *imglist, int index,
                     QList<QGifFrameInfoData> &images,
                     QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

protected:
  GifDecoder *gif;
  int oldindex;
  QList<QGifFrameInfoData> oldimgs;
  QListWidget *imgw;
};

#endif // INSERTFRAMECOMMAND_H
