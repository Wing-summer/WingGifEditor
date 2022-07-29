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

  });
  blist.append(b);

  b = new DButtonBoxButton(tr("Cancel"), this);
  connect(b, &DButtonBoxButton::clicked, [=] {

  });
  blist.append(b);
  btnbox->setButtonList(blist, false);
  addContent(btnbox);
  setFocusPolicy(Qt::StrongFocus);
}
