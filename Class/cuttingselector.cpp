#include "cuttingselector.h"
#include <QGraphicsScene>

CuttingSelector::CuttingSelector(QGraphicsItem *parent)
    : QGraphicsRectItem(parent) {
  setFlags(QGraphicsItem::ItemIsMovable |
           QGraphicsItem::ItemSendsGeometryChanges);
}

void CuttingSelector::setLimitArea(QRectF rect) { limitArea = rect; }

QVariant CuttingSelector::itemChange(QGraphicsItem::GraphicsItemChange change,
                                     const QVariant &value) {

  return QGraphicsItem::itemChange(change, value);
}
