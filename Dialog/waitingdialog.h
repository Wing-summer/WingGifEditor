#ifndef WAITINGDIALG_H
#define WAITINGDIALG_H

#include <DDialog>
#include <DLabel>
#include <DMainWindow>
#include <DWaterProgress>
#include <QPropertyAnimation>
#include <QWidget>

DWIDGET_USE_NAMESPACE

class WaitingDialog : public DDialog {
  Q_OBJECT
public:
  WaitingDialog(DMainWindow *parent = nullptr);

  void start(QString message);
  void stop();
  void setMessage(QString message);

private:
  DWaterProgress *pro;
  QPropertyAnimation *animation;
  DLabel *info;
};

#endif // WAITINGDIALG_H
