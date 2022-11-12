#ifndef SETTINGS_H
#define SETTINGS_H

#include <DMainWindow>
#include <QObject>

DWIDGET_USE_NAMESPACE

class Settings : public QObject {
  Q_OBJECT
public:
  explicit Settings(QObject *parent = nullptr);

  static Settings *instance();

  void saveWindowStatus(DMainWindow *wnd);
  void loadWindowStatus(DMainWindow *wnd);

  void setWindowState(int state);
  int windowState();

  QString loadFileDialogCurrent();
  void saveFileDialogCurrent(QString path);

  void setTerminal(QString path);
  QString terminal();

  void setReprocessingCmdline(QString cmdline);
  QString reprocessingCmdline();

signals:
  void sigreprocessingCmdlineChanged();
  void sigFileDialogCurrentChanged();

private:
  static Settings *s_pSetting;
};

#endif // SETTINGS_H
