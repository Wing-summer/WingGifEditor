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
    Dialog/aboutsoftwaredialog.cpp \
    Class/clipboardhelper.cpp \
    Dialog/cropgifdialog.cpp \
    UndoCommand/moveframecommand.cpp \
    UndoCommand/removeframecommand.cpp \
    UndoCommand/insertframecommand.cpp \
    Class/gifeditor.cpp \
    Class/gifeditorscene.cpp \
    Class/SizeGripItem.cpp \
    Class/cuttingselector.cpp \
    UndoCommand/flipframecommand.cpp \
    UndoCommand/delayframecommand.cpp \
    UndoCommand/rotateframecommand.cpp \
    UndoCommand/reverseframecommand.cpp \
    UndoCommand/delframedircommand.cpp \
    UndoCommand/reduceframecommand.cpp \
    UndoCommand/scaleframecommand.cpp \
    UndoCommand/replaceframecommand.cpp \ 
    UndoCommand/cropimagecommand.cpp \
    Dialog/waitingdialog.cpp \
    settingdialog.cpp \
    Class/settings.cpp

RESOURCES += resources.qrc

HEADERS += Dialog/mainwindow.h \
    Dialog/sponsordialog.h \
    Dialog/newdialog.h \
    Dialog/exportdialog.h \
    Dialog/createreversedialog.h \
    Dialog/scalegifdialog.h \
    Dialog/reduceframedialog.h \
    Class/playgifmanager.h \
    Dialog/aboutsoftwaredialog.h \
    Class/clipboardhelper.h \
    Dialog/cropgifdialog.h \
    UndoCommand/moveframecommand.h \
    UndoCommand/removeframecommand.h \
    UndoCommand/insertframecommand.h \
    Class/gifeditor.h \
    Class/gifeditorscene.h \
    Class/SizeGripItem.h \
    Class/cuttingselector.h \
    UndoCommand/flipframecommand.h \
    UndoCommand/delayframecommand.h \
    UndoCommand/rotateframecommand.h \
    UndoCommand/reverseframecommand.h \
    UndoCommand/delframedircommand.h \
    UndoCommand/reduceframecommand.h \
    UndoCommand/scaleframecommand.h \
    UndoCommand/replaceframecommand.h \ 
    UndoCommand/cropimagecommand.h \
    Dialog/waitingdialog.h \
    Class/settings.h

DISTFILES +=

TRANSLATIONS += \
    $$PWD/lang/zh.ts

include($$PWD/GifImage/WingGif.pri)
