#ifndef INSERTFRAMECOMMAND_H
#define INSERTFRAMECOMMAND_H

#include "GifImage/gifhelper.h"
#include <QUndoCommand>

class InsertFrameCommand : public QUndoCommand {
public:
  InsertFrameCommand(GifHelper *helper, QVector<int> &index,
                     QUndoCommand *parent = nullptr);

  void undo() override;
  void redo() override;

protected:
  GifHelper *gif;
  QVector<int> olds;
};

#endif // INSERTFRAMECOMMAND_H
