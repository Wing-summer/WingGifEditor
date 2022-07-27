#include "cropgifdialog.h"

CropGifDialog::CropGifDialog(DMainWindow *parent) : DDialog(parent) {}

CropResult CropGifDialog::getResult() {}

void CropGifDialog::on_accept() {}

void CropGifDialog::on_reject() { done(0); }

void CropGifDialog::closeEvent(QCloseEvent *event) {
  Q_UNUSED(event);
  done(0);
}
