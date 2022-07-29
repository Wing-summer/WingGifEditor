#ifndef CROPGIFDIALOG_H
#define CROPGIFDIALOG_H

#include <DFloatingWidget>
#include <DLabel>

DWIDGET_USE_NAMESPACE

struct CropResult {
  int x;
  int y;
  int w;
  int h;
};

class CropGifDialog : public DFloatingWidget {
public:
  CropGifDialog(QWidget *parent = nullptr);
  CropResult getResult();

signals:
  void crop(int x, int y, int w, int h);

private:
  CropResult res;
};

#endif // CROPGIFDIALOG_H
