#ifndef GIFEDITORSCENE_H
#define GIFEDITORSCENE_H

#include "SizeGripItem.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class GifEditorScene : public QGraphicsScene {
public:
  GifEditorScene(QPixmap pix, QObject *parent = nullptr);

  bool isCuttingMode();
  void setCuttingMode(bool value);

  void setFrameImg(QPixmap img);

  QRectF contentBounding();

private:
  bool iscutmode = false;
  QGraphicsPixmapItem *picview;
  QGraphicsRectItem *sel;
  SizeGripItem *handler;
  QPointF oldpos;
};

class RectResizer : public SizeGripItem::Resizer {
public:
  void operator()(QGraphicsItem *item, const QRectF &rect) override {
    QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem *>(item);

    if (rectItem) {
      rectItem->setRect(rect);
    }
  }
};

#endif // GIFEDITORSCENE_H
