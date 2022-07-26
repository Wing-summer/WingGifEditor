#include "cuttingselector.h"
#include <QGraphicsScene>

CuttingSelector::CuttingSelector(QGraphicsItem *parent)
    : QGraphicsRectItem(parent) {
  // Issue 解决后取消注释: #I5JR3S
  //  setFlags(QGraphicsItem::ItemIsMovable |
  //           QGraphicsItem::ItemSendsGeometryChanges);
}

void CuttingSelector::setLimitArea(QRectF rect) { limitArea = rect; }

QVariant CuttingSelector::itemChange(QGraphicsItem::GraphicsItemChange change,
                                     const QVariant &value) {

  if (change == ItemPositionChange && scene()) {
    // QPointF newPos = value.toPointF();
  }
  return QGraphicsItem::itemChange(change, value);
}
