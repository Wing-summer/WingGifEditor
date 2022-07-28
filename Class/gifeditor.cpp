#include "gifeditor.h"
#include <QGuiApplication>
#include <QWheelEvent>

GifEditor::GifEditor(QPixmap pix, QWidget *parent) : DGraphicsView(parent) {
  picview = scene.addPixmap(pix);
  picview->setScale(1);
  setScene(&scene);
  rubber = new QRubberBand(QRubberBand::Rectangle, this);
}

void GifEditor::setBackgroudPix(QPixmap pix) {
  picview->setPixmap(pix);
  refreshEditor();
}

void GifEditor::zoomIn() { scale(1.1, 1.1); }

void GifEditor::zoomOut() { scale(0.9, 0.9); }

void GifEditor::setZoom(int value) {
  qreal v = value / 100;
  scale(v, v);
}

void GifEditor::mousePressEvent(QMouseEvent *event) {}

void GifEditor::mouseMoveEvent(QMouseEvent *event) {}

void GifEditor::mouseReleaseEvent(QMouseEvent *event) {}

void GifEditor::wheelEvent(QWheelEvent *event) {
  auto mod = QGuiApplication::keyboardModifiers();
  if (mod != Qt::KeyboardModifier::ControlModifier)
    return;

  QPoint numPixels = event->pixelDelta();
  QPoint numDegrees = event->angleDelta();

  if (!numPixels.isNull()) {
    if (numPixels.y() > 0) {
      zoomOut();
    } else {
      zoomIn();
    }
  } else if (!numDegrees.isNull()) {
    if (numDegrees.y() > 0) {
      zoomOut();
    } else {
      zoomIn();
    }
  }

  event->accept();
}

void GifEditor::refreshEditor() {
  auto r = picview->boundingRect();
  setSceneRect(r);
  fitInView(r, Qt::KeepAspectRatio);
}
