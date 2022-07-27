#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <DButtonBox>
#include <DDialog>
#include <DFileChooserEdit>
#include <DMainWindow>
#include <QObject>

DWIDGET_USE_NAMESPACE

enum class ExportImageType { PNG, JPG, TIFF, WEBP };

struct ExportResult {
  QString path;
  ExportImageType type;
};

class ExportDialog : public DDialog {
public:
  ExportDialog(DMainWindow *parent = nullptr);
  ExportResult getResult();

private:
  void on_accept();
  void on_reject();

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  DFileChooserEdit *folder;
  DButtonBox *btnbox;

  ExportResult res;
};

#endif // EXPORTDIALOG_H
