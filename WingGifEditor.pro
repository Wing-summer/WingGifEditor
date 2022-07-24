QT += core gui dtkwidget

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include($$PWD/QtGifImage/gifimage/qtgifimage.pri)

TARGET = WingGifEditor
TEMPLATE = app

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    gifeditorscene.cpp

RESOURCES += resources.qrc

HEADERS += mainwindow.h \
    gifeditorscene.h
