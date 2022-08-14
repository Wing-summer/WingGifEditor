#ifndef DELFRAMEDIRCOMMAND_H
#define DELFRAMEDIRCOMMAND_H

#include "GifImage/decoder/gifdecoder.h"
#include <QListWidget>
#include <QUndoCommand>

enum class DelDirection { Before, After };

class DelFrameDirCommand : public QUndoCommand {
public:
  DelFrameDirCommand(GifDecoder *helper, int index, DelDirection dir,
                     QListWidget *listw, QUndoCommand *parent = nullptr);
  void undo() override;
  void redo() override;

private:
  GifDecoder *gif;
  int oldindex;
  DelDirection olddir;
  QListWidget *listimg;
  QList<QGifFrameInfoData> oldimgs;
};

#endif // DELFRAMEDIRCOMMAND_H
