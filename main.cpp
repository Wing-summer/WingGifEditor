#include "Dialog/mainwindow.h"
#include <DApplication>
#include <DApplicationSettings>
#include <DMessageBox>
#include <DTitlebar>
#include <DWidgetUtil>
#include <QTranslator>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[]) {
  //解决 root/ubuntu 主题样式走形
  qputenv("XDG_CURRENT_DESKTOP", "Deepin");
  QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

  // 程序内强制添加 -platformtheme
  // deepin 参数喂给 Qt 让 Qt 正确使用 Deepin 主题修复各种奇怪样式问题
  QVector<char *> fakeArgs(argc + 2);
  char fa1[] = "-platformtheme";
  char fa2[] = "deepin";
  fakeArgs[0] = argv[0];
  fakeArgs[1] = fa1;
  fakeArgs[2] = fa2;

  for (int i = 1; i < argc; i++)
    fakeArgs[i + 2] = argv[i];
  int fakeArgc = argc + 2;
  DApplication a(fakeArgc, fakeArgs.data());
  QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);

  auto s = a.applicationDirPath() + "/lang/default.qm";
  QTranslator translator;
  if (!translator.load(s)) {
    DMessageBox::critical(nullptr, "Error", "Error Loading Translation File!",
                          DMessageBox::Ok);
    return -1;
  }

  a.installTranslator(&translator);

  a.setOrganizationName("WingCloud");
  a.setApplicationName("WingGifEditor");
  a.setApplicationVersion("1.0");
  a.setProductIcon(QIcon(":/images/icon.png"));
  a.setProductName(QObject::tr("WingGifEditor"));
  a.setApplicationDescription(
      QObject::tr("A tiny easy2use gif editor for Deepin"));

  a.loadTranslator();
  a.setApplicationDisplayName(QObject::tr("WingGifEditor"));

  // 保存程序的窗口主题设置
  DApplicationSettings as;
  Q_UNUSED(as)

  MainWindow w;
  w.show();
  Dtk::Widget::moveToCenter(&w);
  return a.exec();
}
