#ifndef CREATEREVERSEDIALOG_H
#define CREATEREVERSEDIALOG_H

#include <DDialog>
#include <DDialogButtonBox>
#include <DLabel>
#include <DMainWindow>
#include <DSpinBox>

DWIDGET_USE_NAMESPACE

struct ReverseResult {
  int start = -1;
  int end = -1;
};

class CreateReverseDialog : public DDialog {
  Q_OBJECT
public:
  CreateReverseDialog(int max, DMainWindow *parent = nullptr);
  ReverseResult getResult();

private:
  void on_accept();
  void on_reject();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  DSpinBox *sbfrom, *sbto;
  ReverseResult res;
};

#endif // CREATEREVERSEDIALOG_H
