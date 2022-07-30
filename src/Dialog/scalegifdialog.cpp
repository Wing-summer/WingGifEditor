#include "scalegifdialog.h"
#include <DDialogButtonBox>
#include <DLabel>
#include <QShortcut>

ScaleGIFDialog::ScaleGIFDialog(QSize size, DMainWindow *parent)
    : DDialog(parent) {
  setWindowTitle(tr("ScaleGIFDialog"));
  btnbox = new DButtonBox(this);
  QList<DButtonBoxButton *> blist;
  auto b = new DButtonBoxButton(tr("Unlock"), this);
  connect(b, &DButtonBoxButton::toggled, [=](bool b) {
    if (b)
      _lockscale = false;
  });
  blist.append(b);
  b = new DButtonBoxButton(tr("Lock"), this);
  connect(b, &DButtonBoxButton::toggled, [=](bool b) {
    if (b)
      _lockscale = true;
  });
  blist.prepend(b);
  btnbox->setButtonList(blist, true);
  b->setChecked(true);
  addContent(btnbox);
  addSpacing(10);
  addContent(new DLabel(tr("Width"), this));
  addSpacing(3);

  _oldwidth = size.width();

  sbwidth = new DSpinBox(this);
  sbwidth->setMinimum(1);
  sbwidth->setMaximum(INT_MAX);
  sbwidth->setValue(_oldwidth);
  sbwidth->setSuffix(" px");

  connect(sbwidth, &DSpinBox::editingFinished, this, [=] {
    if (_lockscale)
      sbheight->setValue(sbwidth->value() * _oldheight / _oldwidth);
  });

  addContent(sbwidth);
  addSpacing(5);
  addContent(new DLabel(tr("Height"), this));
  addSpacing(3);

  _oldheight = size.height();

  sbheight = new DSpinBox(this);
  sbheight->setMinimum(1);
  sbheight->setMaximum(INT_MAX);
  sbheight->setValue(_oldheight);
  sbheight->setSuffix(" px");

  connect(sbheight, &DSpinBox::editingFinished, this, [=] {
    if (_lockscale)
      sbwidth->setValue(sbheight->value() * _oldwidth / _oldheight);
  });

  addContent(sbheight);
  addSpacing(20);
  auto dbbox = new DDialogButtonBox(
      DDialogButtonBox::Ok | DDialogButtonBox::Cancel, this);
  connect(dbbox, &DDialogButtonBox::accepted, this, &ScaleGIFDialog::on_accept);
  connect(dbbox, &DDialogButtonBox::rejected, this, &ScaleGIFDialog::on_reject);
  auto key = QKeySequence(Qt::Key_Return);
  auto s = new QShortcut(key, this);
  connect(s, &QShortcut::activated, this, &ScaleGIFDialog::on_accept);
  addContent(dbbox);
}

ScaleResult ScaleGIFDialog::getResult() { return res; }

void ScaleGIFDialog::on_accept() {
  res.width = sbwidth->value();
  res.height = sbheight->value();
  done(1);
}

void ScaleGIFDialog::on_reject() { done(0); }

void ScaleGIFDialog::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);
  done(0);
}
