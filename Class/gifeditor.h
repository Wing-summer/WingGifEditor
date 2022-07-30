#ifndef GIFEDITOR_H
#define GIFEDITOR_H

#include "gifeditorscene.h"
#include <DGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QRubberBand>

DWIDGET_USE_NAMESPACE

class GifEditor : public DGraphicsView {
  Q_OBJECT
public:
  GifEditor(QPixmap pix, QWidget *parent = nullptr);
  void setBackgroudPix(QPixmap pix);

  void zoomIn();
  void zoomOut();
  void setZoom(int value);
  void refreshEditor();

  void initCrop();
  void endCrop();

public slots:
  void setSelRect(int x, int y, int w, int h);

signals:
  void selRectChanged(QRectF rect);

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private:
  GifEditorScene *scene;
  QRubberBand *rubber;

  QPoint tmppos;
  bool iscut = true;
};

#endif // GIFEDITOR_H
