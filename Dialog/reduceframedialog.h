#ifndef REDUCEFRAMEDIALOG_H
#define REDUCEFRAMEDIALOG_H

#include <DDialog>
#include <DDialogButtonBox>
#include <DLabel>
#include <DMainWindow>
#include <DSpinBox>

DWIDGET_USE_NAMESPACE

struct ReduceResult {
  int start = -1;
  int end = -1;
  int stepcount = 1;
};

class ReduceFrameDialog : public DDialog {
public:
  ReduceFrameDialog(int max, DMainWindow *parent = nullptr);
  ReduceResult getResult();

private:
  void on_accept();
  void on_reject();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  DSpinBox *sbcount;
  DSpinBox *sbfrom, *sbto;

  ReduceResult res;
};

#endif // REDUCEFRAMEDIALOG_H
