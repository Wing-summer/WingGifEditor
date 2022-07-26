#ifndef REDUCEFRAMEDIALOG_H
#define REDUCEFRAMEDIALOG_H

#include <DDialog>
#include <DDialogButtonBox>
#include <DLabel>
#include <DMainWindow>
#include <DSpinBox>

DWIDGET_USE_NAMESPACE

class ReduceFrameDialog : public DDialog {
public:
  ReduceFrameDialog(DMainWindow *parent = nullptr);

private:
  void on_accept();
  void on_reject();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  DSpinBox *sbinterval;
  DSpinBox *sbfrom, *sbto;
};

#endif // REDUCEFRAMEDIALOG_H
