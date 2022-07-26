#include "reduceframedialog.h"
#include <QShortcut>

ReduceFrameDialog::ReduceFrameDialog(int max, DMainWindow *parent)
    : DDialog(parent) {
  setWindowTitle(tr("ReduceFrameDialog"));
  addContent(new DLabel(tr("ReduceCount"), this));
  addSpacing(3);
  max--; //转为索引
  sbcount = new DSpinBox(this);
  sbcount->setMinimum(1);
  sbcount->setValue(1);
  sbcount->setMaximum(max - 1);
  addContent(sbcount);
  addSpacing(5);
  addContent(new DLabel(tr("ReduceStart"), this));
  addSpacing(3);
  sbfrom = new DSpinBox(this);
  sbfrom->setMinimum(0);
  sbfrom->setValue(0);
  sbfrom->setMaximum(max - 1);
  addContent(sbfrom);
  addSpacing(5);
  addContent(new DLabel(tr("ReduceEnd"), this));
  sbto = new DSpinBox(this);
  sbto->setMinimum(1);
  sbto->setValue(max);
  sbto->setMaximum(max);
  addContent(sbto);
  addSpacing(20);
  auto dbbox = new DDialogButtonBox(
      DDialogButtonBox::Ok | DDialogButtonBox::Cancel, this);
  connect(dbbox, &DDialogButtonBox::accepted, this,
          &ReduceFrameDialog::on_accept);
  connect(dbbox, &DDialogButtonBox::rejected, this,
          &ReduceFrameDialog::on_reject);
  auto key = QKeySequence(Qt::Key_Return);
  auto s = new QShortcut(key, this);
  connect(s, &QShortcut::activated, this, &ReduceFrameDialog::on_accept);
  addContent(dbbox);
}

ReduceResult ReduceFrameDialog::getResult() { return res; }

void ReduceFrameDialog::on_accept() {
  res.start = sbfrom->value();
  res.end = sbto->value();
  res.stepcount = sbcount->value();
  done(1);
}

void ReduceFrameDialog::on_reject() { done(0); }

void ReduceFrameDialog::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);
  done(0);
}
