#include "waitingdialog.h"
#include <DWidgetUtil>
#include <QHideEvent>
#include <QShowEvent>
#include <QThread>

WaitingDialog::WaitingDialog(DMainWindow *parent) : DDialog(parent) {
  setWindowTitle(tr("PleaseWait"));
  addSpacing(50);
  pro = new DWaterProgress(this);
  pro->setTextVisible(false);
  pro->setValue(0);
  pro->setFixedSize(150, 150);
  addContent(pro, Qt::AlignCenter);

  animation = new QPropertyAnimation(pro, "value");
  animation->setDuration(1000);
  animation->setStartValue(0);
  animation->setEndValue(100);
  animation->setLoopCount(-1);
  animation->setEasingCurve(QEasingCurve::Linear);

  addSpacing(40);
  info = new DLabel(tr("Please Waiting..."), this);
  info->setAlignment(Qt::AlignmentFlag::AlignCenter);
  addContent(info);

  moveToThread(new QThread(this)); // 启用一个新线程，防止界面卡死

  addSpacing(50);
}

void WaitingDialog::start(QString message) {
  info->setText(message);
  pro->start();
  animation->start();
  Dtk::Widget::moveToCenter(this);
  show();
}

void WaitingDialog::stop() {
  pro->stop();
  animation->stop();
}
