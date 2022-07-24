#include "gifeditorscene.h"
#include <QGraphicsView>

GifEditorScene::GifEditorScene(QObject *parent) : QGraphicsScene(parent) {}

void GifEditorScene::setBackgroundImage(QImage img) {
  m_back = QPixmap::fromImage(img);
}

void GifEditorScene::drawBackground(QPainter *painter, const QRectF &rect) {
  Q_UNUSED(rect)
  if (views().count() == 0)
    return;
  QGraphicsView *pView = views().first();
  QRect contentRect = pView->viewport()->contentsRect();
  QRectF sceneRect = pView->mapToScene(contentRect).boundingRect();
  painter->drawPixmap(sceneRect, m_back, QRect());
}
