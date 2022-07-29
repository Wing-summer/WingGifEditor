#ifndef CROPGIFDIALOG_H
#define CROPGIFDIALOG_H

#include <DButtonBox>
#include <DDialog>
#include <DLabel>
#include <DMainWindow>
#include <DSpinBox>

DWIDGET_USE_NAMESPACE

class CropGifDialog : public DDialog {
public:
  CropGifDialog(DMainWindow *parent = nullptr);

signals:
  void crop(int x, int y, int w, int h);

private:
  DButtonBox *btnbox;
  DSpinBox *m_x, *m_y, *m_w, *m_h;
};

#endif // CROPGIFDIALOG_H
