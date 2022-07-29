#include "gifeditor.h"
#include <QGuiApplication>
#include <QWheelEvent>

GifEditor::GifEditor(QPixmap pix, QWidget *parent) : DGraphicsView(parent) {
  scene = new GifEditorScene(pix, this);
  setScene(scene);
  rubber = new QRubberBand(QRubberBand::Line, this);
  scale(1, 1);
}

void GifEditor::setBackgroudPix(QPixmap pix) { scene->setFrameImg(pix); }

void GifEditor::zoomIn() { scale(1.1, 1.1); }

void GifEditor::zoomOut() { scale(0.9, 0.9); }

void GifEditor::setZoom(int value) {
  qreal v = value / 100;
  scale(v, v);
}

void GifEditor::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::MouseButton::LeftButton &&
      !scene->isCuttingMode()) {
    tmppos = event->pos();

    rubber->setGeometry(QRect(tmppos, QSize()));
    rubber->show();
  }
  DGraphicsView::mousePressEvent(event);
}

void GifEditor::mouseMoveEvent(QMouseEvent *event) {
  rubber->setGeometry(QRect(tmppos, event->pos()).normalized());
  DGraphicsView::mouseMoveEvent(event);
}

void GifEditor::mouseReleaseEvent(QMouseEvent *event) {
  Q_UNUSED(event);
  rubber->hide();
  DGraphicsView::mouseReleaseEvent(event);
}

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
  auto r = scene->contentBounding();
  setSceneRect(r);
  fitInView(r, Qt::KeepAspectRatio);
}

void GifEditor::initCrop() { scene->setCuttingMode(true); }
