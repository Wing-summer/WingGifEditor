TARGET = QtGifImage

load(qt_module)

CONFIG += build_gifimage_lib

include(qtgifimage.pri)

QMAKE_TARGET_COMPANY = "Debao Zhang"
QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2013 Debao Zhang <hello@debao.me>"
QMAKE_TARGET_DESCRIPTION = ".gif file reader and wirter for Qt"

# 这些是文档的部分
OTHER_FILES += $$QMAKE_DOCS doc/src/*.qdoc
QMAKE_DOCS = $$PWD/doc/qtgifimage.qdocconf
