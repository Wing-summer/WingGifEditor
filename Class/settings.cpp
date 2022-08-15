#include "settings.h"
#include <DMessageManager>
#include <DSettingsOption>
#include <QApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QStyleFactory>

Settings *Settings::s_pSetting = nullptr;

Settings::Settings(QObject *parent) : QObject(parent) {
  QString strConfigPath =
      QString("%1/%2/%3/config.conf")
          .arg(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation))
          .arg(qApp->organizationName())
          .arg(qApp->applicationName());

  m_backend = new QSettingBackend(strConfigPath);

  settings = DSettings::fromJsonFile(":/settings.json");
  settings->setBackend(m_backend);

#define BindConfigSignal(Var, SettingName, Signal)                             \
  auto Var = settings->option(SettingName);                                    \
  connect(Var, &Dtk::Core::DSettingsOption::valueChanged,                      \
          this, [=](QVariant value) Signal);

  auto ditherer = settings->option("editor.image.ditherer");
  QMap<QString, QVariant> dithererMap;
  dithererMap.insert("keys", QStringList() << "No"
                                           << "M2"
                                           << "Bayer"
                                           << "FloydSteinberg");

  dithererMap.insert("values", QStringList()
                                   << tr("No") << tr("M2") << tr("Bayer")
                                   << tr("FloydSteinberg"));
  ditherer->setData("items", dithererMap);

  auto quantizer = settings->option("editor.image.quantizer");
  QMap<QString, QVariant> quantizerMap;
  quantizerMap.insert("keys", QStringList() << "Uniform"
                                            << "MedianCut"
                                            << "KMeans"
                                            << "Random"
                                            << "Octree"
                                            << "NeuQuant");

  quantizerMap.insert("values", QStringList()
                                    << tr("Uniform") << tr("MedianCut")
                                    << tr("KMeans") << tr("Random")
                                    << tr("Octree") << tr("NeuQuant"));
  quantizer->setData("items", quantizerMap);

  auto windowState = settings->option("appearance.window.windowsize");
  QMap<QString, QVariant> windowStateMap;
  windowStateMap.insert("keys", QStringList() << "window_normal"
                                              << "window_maximum"
                                              << "window_minimum"
                                              << "fullscreen");
  windowStateMap.insert("values", QStringList()
                                      << tr("Normal") << tr("Maximum")
                                      << tr("Minimum") << tr("Fullscreen"));
  windowState->setData("items", windowStateMap);
}

Settings::~Settings() {}

void Settings::setSettingDialog(DSettingsDialog *settingsDialog) {
  m_pSettingsDialog = settingsDialog;
}

void Settings::applySetting() {

#define Apply(Var, SettingName, Signal)                                        \
  auto Var = settings->option(SettingName);                                    \
  if (Var != nullptr)                                                          \
    emit Signal;

  Apply(ditherer, "editor.image.ditherer",
        sigAdjustDither(ditherer->value().toString()));
  Apply(quantizer, "editor.image.ditherer",
        sigAdjustQuantizer(quantizer->value().toString()));
  Apply(windowstate, "appearance.window.windowsize",
        sigChangeWindowState(windowstate->value().toString()));
}

// This function is workaround, it will remove after DTK fixed SettingDialog
// theme bug.
void Settings::dtkThemeWorkaround(QWidget *parent, const QString &theme) {
  parent->setStyle(QStyleFactory::create(theme));

  for (auto obj : parent->children()) {
    auto w = qobject_cast<QWidget *>(obj);
    if (!w) {
      continue;
    }

    dtkThemeWorkaround(w, theme);
  }
}

Settings *Settings::instance() {
  if (s_pSetting == nullptr) {
    s_pSetting = new Settings;
  }
  return s_pSetting;
}

DDialog *Settings::createDialog(const QString &title, const QString &content,
                                const bool &bIsConflicts) {
  DDialog *dialog = new DDialog(title, content, m_pSettingsDialog);
  dialog->setWindowFlags(dialog->windowFlags() | Qt::WindowStaysOnBottomHint);
  dialog->setIcon(QIcon(":/images/setting.png"));

  if (bIsConflicts) {
    dialog->addButton(QString(tr("Cancel")), true, DDialog::ButtonNormal);
    dialog->addButton(QString(tr("Replace")), false, DDialog::ButtonRecommend);
  } else {
    dialog->addButton(QString(tr("OK")), true, DDialog::ButtonRecommend);
  }

  return dialog;
}

void Settings::saveWindowState(DMainWindow *wnd) {
  if (wnd != nullptr) {
    QSettings settings(QApplication::organizationName(),
                       QApplication::applicationName());
    settings.setValue("geometry", wnd->saveGeometry());
    settings.setValue("windowState", wnd->saveState());
  }
}

void Settings::loadWindowState(DMainWindow *wnd) {
  if (wnd != nullptr) {
    QSettings settings(QApplication::organizationName(),
                       QApplication::applicationName());
    wnd->restoreGeometry(settings.value("geometry").toByteArray());
    wnd->restoreState(settings.value("windowState").toByteArray());
  }
}

void Settings::saveFileDialogCurrent(QString path) {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  settings.setValue("curpath", path);
}

QString Settings::loadFileDialogCurrent() {
  QSettings settings(QApplication::organizationName(),
                     QApplication::applicationName());
  return settings.value("curpath").toString();
}
