#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <DButtonBox>
#include <DDialog>
#include <DLabel>
#include <DListWidget>
#include <DMainWindow>
#include <QHBoxLayout>
#include <QObject>
#include <QVBoxLayout>

DWIDGET_USE_NAMESPACE

enum class NewType { FromPics, FromGifs };

class NewDialog : public DDialog {
  Q_OBJECT
public:
  NewDialog(NewType type, DMainWindow *parent = nullptr);
  QStringList getResult();

private:
  void on_accept();
  void on_reject();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  QStringList filenames;

  DButtonBox *btnbox;
  DListWidget *imgslist;
  DLabel *imgview;
  DLabel *imgsize;
};

#endif // NEWDIALOG_H
