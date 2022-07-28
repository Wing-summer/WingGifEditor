#ifndef GIFEDITOR_H
#define GIFEDITOR_H

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

protected:
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private:
  void refreshEditor();

private:
  QGraphicsScene scene;
  QGraphicsPixmapItem *picview;
  QRubberBand *rubber;
};

#endif // GIFEDITOR_H
