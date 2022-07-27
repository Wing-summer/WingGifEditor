QT += core gui dtkwidget

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WingGifEditor
TEMPLATE = app

SOURCES += \
        main.cpp \
    Dialog/mainwindow.cpp \
    Dialog/sponsordialog.cpp \
    Dialog/newdialog.cpp \
    Dialog/exportdialog.cpp \
    Dialog/createreversedialog.cpp \
    Dialog/scalegifdialog.cpp \
    Dialog/reduceframedialog.cpp \
    Class/playgifmanager.cpp \
    GifImage/gifimage.cpp \
    GifImage/gifhelper.cpp \
    Dialog/aboutsoftwaredialog.cpp \
    Class/clipboardhelper.cpp

RESOURCES += resources.qrc

HEADERS += Dialog/mainwindow.h \
    Dialog/sponsordialog.h \
    Dialog/newdialog.h \
    Dialog/exportdialog.h \
    Dialog/createreversedialog.h \
    Dialog/scalegifdialog.h \
    Dialog/reduceframedialog.h \
    Class/playgifmanager.h \
    GifImage/gifimage.h \
    GifImage/gifhelper.h \
    Dialog/aboutsoftwaredialog.h \
    Class/clipboardhelper.h

QMAKE_CXXFLAGS += $(shell Magick++-config --cppflags --cxxflags)

LIBS += $(shell Magick++-config --ldflags --libs)
