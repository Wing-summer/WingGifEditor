QT += core gui dtkwidget

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include($$PWD/QtGifImage/gifimage/qtgifimage.pri)

TARGET = WingGifEditor
TEMPLATE = app

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    sponsordialog.cpp \
    newdialog.cpp \
    exportdialog.cpp \
    createreversedialog.cpp \
    scalegifdialog.cpp

RESOURCES += resources.qrc

HEADERS += mainwindow.h \
    sponsordialog.h \
    newdialog.h \
    exportdialog.h \
    createreversedialog.h \
    scalegifdialog.h
