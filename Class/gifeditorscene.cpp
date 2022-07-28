#include "gifeditorscene.h"
#include <QGraphicsSceneMouseEvent>

GifEditorScene::GifEditorScene(QPixmap pix, QObject *parent)
    : QGraphicsScene(parent) {
  picview = addPixmap(pix);
  sel = new QGraphicsRectItem;
  auto brush = QBrush(Qt::red);
  auto pen = QPen(brush, 4);
  sel->setPen(pen);
  sel->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemClipsToShape |
                QGraphicsItem::ItemIsFocusable);
  handler = new SizeGripItem(new RectResizer, sel);
  sel->setFocus();
  sel->hide();
  addItem(sel);
}

bool GifEditorScene::isCuttingMode() { return iscutmode; }

void GifEditorScene::setCuttingMode(bool value) {
  iscutmode = value;
  if (value) {
    auto pix = picview->pixmap();
    QRectF rect(0, 0, pix.width(), pix.height());
    sel->setRect(rect);
    handler->setRect(rect);
    sel->show();
  } else {
    sel->hide();
  }
  handler->setVisible(value);
}

void GifEditorScene::setFrameImg(QPixmap img) { picview->setPixmap(img); }

QRectF GifEditorScene::contentBounding() { return picview->boundingRect(); }
