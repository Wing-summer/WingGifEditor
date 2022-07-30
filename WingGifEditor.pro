QT += core gui dtkwidget

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WingGifEditor
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/Dialog/mainwindow.cpp \
    src/Dialog/sponsordialog.cpp \
    src/Dialog/newdialog.cpp \
    src/Dialog/exportdialog.cpp \
    src/Dialog/createreversedialog.cpp \
    src/Dialog/scalegifdialog.cpp \
    src/Dialog/reduceframedialog.cpp \
    src/Class/playgifmanager.cpp \
    src/GifImage/gifimage.cpp \
    src/GifImage/gifhelper.cpp \
    src/Dialog/aboutsoftwaredialog.cpp \
    src/Class/clipboardhelper.cpp \
    src/Dialog/cropgifdialog.cpp \
    src/UndoCommand/moveframecommand.cpp \
    src/UndoCommand/removeframecommand.cpp \
    src/UndoCommand/insertframecommand.cpp \
    src/Class/gifeditor.cpp \
    src/Class/gifeditorscene.cpp \
    src/Class/SizeGripItem.cpp \
    src/Class/cuttingselector.cpp \
    src/UndoCommand/flipframecommand.cpp \
    src/UndoCommand/delayframecommand.cpp \
    src/UndoCommand/rotateframecommand.cpp \
    src/UndoCommand/reverseframecommand.cpp \
    src/UndoCommand/delframedircommand.cpp \
    src/UndoCommand/reduceframecommand.cpp \
    src/UndoCommand/scaleframecommand.cpp \
    src/UndoCommand/replaceframecommand.cpp

RESOURCES += resources/resources.qrc

INCLUDEPATH += src
HEADERS += \
    src/Dialog/mainwindow.h \
    src/Dialog/sponsordialog.h \
    src/Dialog/newdialog.h \
    src/Dialog/exportdialog.h \
    src/Dialog/createreversedialog.h \
    src/Dialog/scalegifdialog.h \
    src/Dialog/reduceframedialog.h \
    src/Class/playgifmanager.h \
    src/GifImage/gifimage.h \
    src/GifImage/gifhelper.h \
    src/Dialog/aboutsoftwaredialog.h \
    src/Class/clipboardhelper.h \
    src/Dialog/cropgifdialog.h \
    src/UndoCommand/moveframecommand.h \
    src/UndoCommand/removeframecommand.h \
    src/UndoCommand/insertframecommand.h \
    src/Class/gifeditor.h \
    src/Class/gifeditorscene.h \
    src/Class/SizeGripItem.h \
    src/Class/cuttingselector.h \
    src/UndoCommand/flipframecommand.h \
    src/UndoCommand/delayframecommand.h \
    src/UndoCommand/rotateframecommand.h \
    src/UndoCommand/reverseframecommand.h \
    src/UndoCommand/delframedircommand.h \
    src/UndoCommand/reduceframecommand.h \
    src/UndoCommand/scaleframecommand.h \
    src/UndoCommand/replaceframecommand.h

QMAKE_CXXFLAGS += $(shell Magick++-config --cppflags --cxxflags)

LIBS += $(shell Magick++-config --ldflags --libs)

DISTFILES +=

TRANSLATIONS += \
    $$PWD/lang/zh.ts
