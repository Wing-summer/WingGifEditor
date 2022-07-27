#ifndef SCALEGIFDIALOG_H
#define SCALEGIFDIALOG_H

#include <DButtonBox>
#include <DDialog>
#include <DMainWindow>
#include <DSpinBox>
#include <QObject>

DWIDGET_USE_NAMESPACE

struct ScaleResult {
  int width;
  int height;
};

class ScaleGIFDialog : public DDialog {
public:
  ScaleGIFDialog(QSize size, DMainWindow *parent = nullptr);
  ScaleResult getResult();

private:
  void on_accept();
  void on_reject();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  DButtonBox *btnbox;
  DSpinBox *sbwidth;
  DSpinBox *sbheight;
  bool _lockscale = true;
  int _oldwidth = 0, _oldheight = 0;
  ScaleResult res;
};

#endif // SCALEGIFDIALOG_H
