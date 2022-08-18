#ifndef SETTINGS_H
#define SETTINGS_H

#include "GifImage/encoder/GifEncoder.h"
#include <DDialog>
#include <DMainWindow>
#include <DSettingsDialog>
#include <QObject>
#include <qsettingbackend.h>

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE
DTK_USE_NAMESPACE

class Settings : public QObject {
  Q_OBJECT
public:
  explicit Settings(QObject *parent = nullptr);
  ~Settings();

  void dtkThemeWorkaround(QWidget *parent, const QString &theme);
  static Settings *instance();

  void setSettingDialog(DSettingsDialog *settingsDialog);
  void applySetting();
  void saveWindowState(DMainWindow *wnd);
  void loadWindowState(DMainWindow *wnd);
  QString loadFileDialogCurrent();
  void saveFileDialogCurrent(QString path);

  DSettings *settings;

signals:
  void sigAdjustQuality(int quality);
  void sigChangeWindowState(QString state);

private:
  DDialog *createDialog(const QString &title, const QString &content,
                        const bool &bIsConflicts);

private:
  Dtk::Core::QSettingBackend *m_backend;
  DSettingsDialog *m_pSettingsDialog;
  static Settings *s_pSetting;
  DDialog *m_pDialog;
};

#endif // SETTINGS_H
