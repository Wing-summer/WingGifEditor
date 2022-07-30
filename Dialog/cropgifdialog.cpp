#include "cropgifdialog.h"
#include <QHBoxLayout>

CropGifDialog::CropGifDialog(DMainWindow *parent) : DDialog(parent) {
  setWindowTitle(tr("CropGifDialog"));

  addContent(new DLabel(tr("X : "), this));
  addSpacing(6);
  m_x = new DSpinBox(this);
  addContent(m_x);
  addSpacing(10);
  addContent(new DLabel(tr("Y : "), this));
  addSpacing(6);
  m_y = new DSpinBox(this);
  addContent(m_y);
  addSpacing(10);
  addContent(new DLabel(tr("W : "), this));
  addSpacing(6);
  m_w = new DSpinBox(this);
  addContent(m_w);
  addContent(new DLabel(tr("H : "), this));
  m_h = new DSpinBox(this);
  addContent(m_h);
  addSpacing(10);

  btnbox = new DButtonBox(this);
  QList<DButtonBoxButton *> blist;

  auto b = new DButtonBoxButton(tr("Cut"), this);
  connect(b, &DButtonBoxButton::clicked, [=] {
    emit this->crop(m_x->value(), m_y->value(), m_w->value(), m_h->value());
    this->close();
  });
  blist.append(b);

  b = new DButtonBoxButton(tr("Cancel"), this);
  connect(b, &DButtonBoxButton::clicked, [=] {
    emit this->pressCancel();
    this->close();
  });
  blist.append(b);
  btnbox->setButtonList(blist, false);
  addContent(btnbox);
  setFocusPolicy(Qt::StrongFocus);
}

void CropGifDialog::setMaxSize(QSize maxsize) { m_max = maxsize; }

void CropGifDialog::setSelRect(QRectF rect) {
  auto x = qMin(qMax(int(rect.x()), 0), m_max.width());
  auto y = qMin(qMax(int(rect.y()), 0), m_max.height());
  auto w = qMin(qMax(int(rect.width()), 0), m_max.width() - x);
  auto h = qMin(qMax(int(rect.height()), 0), m_max.height() - y);
  m_x->setValue(x);
  m_y->setValue(y);
  m_w->setValue(w);
  m_h->setValue(h);
  emit selRectChanged(x, y, w, h);
}
