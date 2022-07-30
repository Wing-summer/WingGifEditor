#include "cropgifdialog.h"
#include <QHBoxLayout>

CropGifDialog::CropGifDialog(DMainWindow *parent) : DDialog(parent) {
  setWindowTitle(tr("CropGifDialog"));

  addContent(new DLabel(tr("X : "), this));
  addSpacing(6);
  m_x = new DSpinBox(this);
  connect(m_x, &DSpinBox::editingFinished, this, [=] {
    m_w->setMaximum(m_max.width() - m_x->value());
    sigSelRectChanged();
  });
  addContent(m_x);
  addSpacing(10);
  addContent(new DLabel(tr("Y : "), this));
  addSpacing(6);
  m_y = new DSpinBox(this);
  connect(m_y, &DSpinBox::editingFinished, this, [=] {
    m_h->setMaximum(m_max.height() - m_y->value());
    sigSelRectChanged();
  });
  addContent(m_y);
  addSpacing(10);
  addContent(new DLabel(tr("W : "), this));
  addSpacing(6);
  m_w = new DSpinBox(this);
  connect(m_w, &DSpinBox::editingFinished, this, [=] { sigSelRectChanged(); });
  addContent(m_w);
  addSpacing(10);
  addContent(new DLabel(tr("H : "), this));
  addSpacing(6);
  m_h = new DSpinBox(this);
  connect(m_h, &DSpinBox::editingFinished, this, [=] { sigSelRectChanged(); });
  addContent(m_h);
  addSpacing(20);

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

void CropGifDialog::setMaxSize(QSize maxsize) {
  m_max = maxsize;
  m_x->setMaximum(maxsize.width());
  m_y->setMaximum(maxsize.height());
  m_w->setMaximum(maxsize.width());
  m_h->setMaximum(maxsize.height());
}

void CropGifDialog::sigSelRectChanged() {
  auto x = m_x->value();
  auto y = m_y->value();
  auto w = m_w->value();
  auto h = m_h->value();
  emit selRectChanged(x, y, w, h);
}

void CropGifDialog::setSelRect(QRectF rect) {
  auto x = qMin(qMax(int(rect.x()), 0), m_max.width());
  auto y = qMin(qMax(int(rect.y()), 0), m_max.height());
  auto w = qMin(qMax(int(rect.width()), 0), m_max.width() - x);
  auto h = qMin(qMax(int(rect.height()), 0), m_max.height() - y);
  m_x->setValue(x);
  m_y->setValue(y);
  m_w->setMaximum(m_max.width() - x);
  m_h->setMaximum(m_max.height() - y);
  m_w->setValue(w);
  m_h->setValue(h);
  emit selRectChanged(x, y, w, h);
}
