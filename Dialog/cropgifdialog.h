#ifndef CROPGIFDIALOG_H
#define CROPGIFDIALOG_H

#include <DDialog>
#include <DDialogButtonBox>
#include <DLabel>
#include <DMainWindow>
#include <DSpinBox>

DWIDGET_USE_NAMESPACE

struct CropResult {
  int x;
  int y;
  int w;
  int h;
};

class CropGifDialog : public DDialog {
public:
  CropGifDialog(DMainWindow *parent = nullptr);
  CropResult getResult();

private:
  void on_accept();
  void on_reject();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  CropResult res;
};

#endif // CROPGIFDIALOG_H
