#ifndef CUTTINGSELECTOR_H
#define CUTTINGSELECTOR_H

#include <QGraphicsRectItem>

class CuttingSelector : public QGraphicsRectItem {
public:
  CuttingSelector(QGraphicsItem *parent = nullptr);
  void setLimitArea(QRectF rect);

protected:
  QVariant itemChange(GraphicsItemChange change,
                      const QVariant &value) override;

private:
  QRectF limitArea;
};

#endif // CUTTINGSELECTOR_H
