#include "createreversedialog.h"
#include <QShortcut>

CreateReverseDialog::CreateReverseDialog(int max, DMainWindow *parent)
    : DDialog(parent) {
  setWindowTitle(tr("CreateReverseDialog"));
  max--; //转为索引
  addContent(new DLabel(tr("Start"), this));
  addSpacing(3);
  sbfrom = new DSpinBox(this);
  sbfrom->setMinimum(0);
  sbfrom->setValue(0);
  sbfrom->setMaximum(max - 1);
  addContent(sbfrom);
  addSpacing(5);
  addContent(new DLabel(tr("End"), this));
  sbto = new DSpinBox(this);
  sbto->setMinimum(1);
  sbto->setValue(max);
  sbto->setMaximum(max);
  addContent(sbto);
  addSpacing(20);
  auto dbbox = new DDialogButtonBox(
      DDialogButtonBox::Ok | DDialogButtonBox::Cancel, this);
  connect(dbbox, &DDialogButtonBox::accepted, this,
          &CreateReverseDialog::on_accept);
  connect(dbbox, &DDialogButtonBox::rejected, this,
          &CreateReverseDialog::on_reject);
  auto key = QKeySequence(Qt::Key_Return);
  auto s = new QShortcut(key, this);
  connect(s, &QShortcut::activated, this, &CreateReverseDialog::on_accept);
  addContent(dbbox);
}

ReverseResult CreateReverseDialog::getResult() { return res; }

void CreateReverseDialog::on_accept() {
  res.start = sbfrom->value();
  res.end = sbto->value();
  done(1);
}

void CreateReverseDialog::on_reject() { done(0); }

void CreateReverseDialog::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);
  done(0);
}
