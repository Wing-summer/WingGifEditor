#include "settingdialog.h"
#include <DDialogButtonBox>
#include <DLabel>
#include <QSettings>
#include <QShortcut>

SettingDialog::SettingDialog(SettingResult res, DMainWindow *parent)
    : DDialog(parent) {
  setWindowTitle(tr("SettingDialog"));
  addContent(new DLabel(tr("WinState"), this));
  addSpacing(3);
  cbWinState = new DComboBox(this);
  cbWinState->addItems({tr("Normal"), tr("Maximum"), tr("Minimum")});
  cbWinState->setCurrentIndex(res.state);
  addContent(cbWinState);
  addSpacing(5);
  addContent(new DLabel(tr("Terminal"), this));
  addSpacing(3);
  leCmd = new DLineEdit(this);
  leCmd->setText(res.terminal);
  addContent(leCmd);
  addSpacing(5);
  addContent(new DLabel(tr("Reprocessing"), this));
  addSpacing(3);
  cmdline = new DLineEdit(this);
  cmdline->setText(res.cmdline);
  addContent(cmdline);
  addSpacing(20);
  auto dbbox = new DDialogButtonBox(
      DDialogButtonBox::Ok | DDialogButtonBox::Cancel, this);
  connect(dbbox, &DDialogButtonBox::accepted, this, &SettingDialog::on_accept);
  connect(dbbox, &DDialogButtonBox::rejected, this, &SettingDialog::on_reject);
  auto key = QKeySequence(Qt::Key_Return);
  auto s = new QShortcut(key, this);
  connect(s, &QShortcut::activated, this, &SettingDialog::on_accept);
  addContent(dbbox);
}

SettingResult SettingDialog::getResult() { return m_res; }

void SettingDialog::on_accept() {
  m_res.state = WinState(cbWinState->currentIndex());
  m_res.terminal = leCmd->text().trimmed();
  m_res.cmdline = cmdline->text().trimmed();
  done(1);
}

void SettingDialog::on_reject() { done(0); }

void SettingDialog::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);
  done(0);
}
