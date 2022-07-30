#ifndef GIFEDITORSCENE_H
#define GIFEDITORSCENE_H

#include "SizeGripItem.h"
#include "cuttingselector.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>

class GifEditorScene : public QGraphicsScene {
  Q_OBJECT
public:
  GifEditorScene(QPixmap pix, QObject *parent = nullptr);

  bool isCuttingMode();
  void setCuttingMode(bool value);

  void setFrameImg(QPixmap img);

  QRectF contentBounding();

public slots:
  void setSelRect(int x, int y, int w, int h);

signals:
  void selRectChanged(QRectF rect);

private:
  bool iscutmode = false;
  QGraphicsPixmapItem *picview;
  CuttingSelector *sel;
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
