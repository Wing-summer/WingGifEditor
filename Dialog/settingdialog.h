#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <DComboBox>
#include <DDialog>
#include <DLineEdit>
#include <DMainWindow>
#include <QObject>

DWIDGET_USE_NAMESPACE

enum WinState { Normal, Maximum, Minimum };

struct SettingResult {
  WinState state = Normal;
  QString terminal;
  QString cmdline;
};

class SettingDialog : public DDialog {
  Q_OBJECT
public:
  explicit SettingDialog(SettingResult res, DMainWindow *parent = nullptr);

  SettingResult getResult();

private:
  void on_accept();
  void on_reject();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  DLineEdit *leCmd;
  DLineEdit *cmdline;
  DComboBox *cbWinState;

  SettingResult m_res;
};

#endif // SETTINGDIALOG_H
