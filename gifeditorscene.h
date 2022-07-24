#ifndef GIFEDITOR_H
#define GIFEDITOR_H

#include <QGraphicsScene>

class GifEditorScene : public QGraphicsScene {
  Q_OBJECT
public:
  GifEditorScene(QObject *parent = nullptr);

  void setBackgroundImage(QImage img);

protected:
  void drawBackground(QPainter *painter, const QRectF &rect) override;

  QPixmap m_back;
};

#endif // GIFEDITOR_H
