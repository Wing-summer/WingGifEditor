#include "mainwindow.h"
#include <DApplication>
#include <DApplicationSettings>
#include <DTitlebar>
#include <DWidgetUtil>

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[]) {
  QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
  DApplication a(argc, argv);
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
