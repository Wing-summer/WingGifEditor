#include "gifeditorscene.h"
#include <QGraphicsSceneMouseEvent>

GifEditorScene::GifEditorScene(QPixmap pix, QObject *parent)
    : QGraphicsScene(parent) {
  picview = addPixmap(pix);
  sel = new CuttingSelector(picview);
  auto brush = QBrush(Qt::red);
  auto pen = QPen(brush, 4);
  sel->setPen(pen);
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
    sel->setLimitArea(rect);
    handler->setRect(rect);
    sel->show();
  } else {
    sel->hide();
  }
  handler->setVisible(value);
}

void GifEditorScene::setFrameImg(QPixmap img) {
  picview->setPixmap(img);
  picview->setPos(0, 0);
  setSceneRect(0, 0, img.width(), img.height());
}

QRectF GifEditorScene::contentBounding() { return picview->boundingRect(); }

void GifEditorScene::setSelRect(int x, int y, int w, int h) {
  handler->setLeft(x);
  handler->setTop(y);
  handler->setRight(x + w);
  handler->setBottom(y + h);
}
