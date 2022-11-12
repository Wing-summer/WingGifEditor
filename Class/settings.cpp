#include "settings.h"
#include <QApplication>
#include <QSettings>

Settings *Settings::s_pSetting = nullptr;

Settings::Settings(QObject *parent) : QObject(parent) {}

void Settings::saveWindowStatus(DMainWindow *wnd) {
  if (wnd != nullptr) {
    QSettings settings(QApplication::organizationName(),
                       QApplication::applicationName());
    settings.setValue("geometry", wnd->saveGeometry());
    settings.setValue("windowState", wnd->saveState());
  }
}

Settings *Settings::instance() {
  if (s_pSetting == nullptr) {
    s_pSetting = new Settings;
  }
  return s_pSetting;
}

void Settings::loadWindowStatus(DMainWindow *wnd) {
  if (wnd != nullptr) {
    QSettings settings(QApplication::organizationName(),
                       QApplication::applicationName());
    wnd->restoreGeometry(settings.value("geometry").toByteArray());
    wnd->restoreState(settings.value("windowState").toByteArray());
  }
}

void Settings::setWindowState(int state) {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  settings.setValue("state", state);
}

int Settings::windowState() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  return settings.value("state", 0).toInt();
}

void Settings::saveFileDialogCurrent(QString path) {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  settings.setValue("curpath", path);
}

void Settings::setTerminal(QString path) {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  settings.setValue("terminal", path);
}

QString Settings::terminal() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  return settings.value("terminal", "/bin/bash").toString().trimmed();
}

void Settings::setReprocessingCmdline(QString cmdline) {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  settings.setValue("cmdline", cmdline);
}

QString Settings::reprocessingCmdline() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  return settings.value("cmdline").toString().trimmed();
}

QString Settings::loadFileDialogCurrent() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  return settings.value("curpath").toString();
}
