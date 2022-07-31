#ifndef CROPGIFDIALOG_H
#define CROPGIFDIALOG_H

#include <DButtonBox>
#include <DDialog>
#include <DLabel>
#include <DMainWindow>
#include <DSpinBox>

DWIDGET_USE_NAMESPACE

class CropGifDialog : public DDialog {
  Q_OBJECT
public:
  CropGifDialog(DMainWindow *parent = nullptr);

  void setMaxSize(QSize maxsize);
  void sigSelRectChanged();

public slots:
  void setSelRect(QRectF rect);

signals:
  void crop(int x, int y, int w, int h);
  void selRectChanged(int x, int y, int w, int h);
  void pressCancel();

private:
  DButtonBox *btnbox;
  DSpinBox *m_x, *m_y, *m_w, *m_h;
  QSize m_max;
};

#endif // CROPGIFDIALOG_H
