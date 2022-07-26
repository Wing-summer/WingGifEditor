QT += core gui dtkwidget

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WingGifEditor
TEMPLATE = app

SOURCES += \
        main.cpp \
    mainwindow.cpp \
    sponsordialog.cpp \
    newdialog.cpp \
    exportdialog.cpp \
    createreversedialog.cpp \
    scalegifdialog.cpp \
    reduceframedialog.cpp \
    playgifmanager.cpp \
    GifImage/gifimage.cpp \
    GifImage/gifhelper.cpp

RESOURCES += resources.qrc

HEADERS += mainwindow.h \
    sponsordialog.h \
    newdialog.h \
    exportdialog.h \
    createreversedialog.h \
    scalegifdialog.h \
    reduceframedialog.h \
    playgifmanager.h \
    GifImage/gifimage.h \
    GifImage/gifhelper.h

QMAKE_CXXFLAGS += $(shell Magick++-config --cppflags --cxxflags)

LIBS += $(shell Magick++-config --ldflags --libs)
