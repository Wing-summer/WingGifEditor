#ifndef DELFRAMEDIRCOMMAND_H
#define DELFRAMEDIRCOMMAND_H

#include "GifImage/gifhelper.h"
#include <QListWidget>
#include <QUndoCommand>

enum class DelDirection { Before, After };

class DelFrameDirCommand : public QUndoCommand {
public:
  DelFrameDirCommand(GifHelper *helper, int index, DelDirection dir,
                     QListWidget *listw, QUndoCommand *parent = nullptr);
  void undo() override;
  void redo() override;

private:
  GifHelper *gif;
  int oldindex;
  DelDirection olddir;
  QListWidget *listimg;
  QVector<Magick::Image> oldimgs;
};

#endif // DELFRAMEDIRCOMMAND_H
