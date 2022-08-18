SOURCES += \
    $$PWD/decoder/gifdecoder.cpp \
    $$PWD/encoder/GifEncoder.cpp \
    $$PWD/encoder/algorithm/NeuQuant.cpp

HEADERS += \
    $$PWD/decoder/gifdecoder.h \
    $$PWD/encoder/GifEncoder.h \
    $$PWD/encoder/algorithm/NeuQuant.h

include($$PWD/giflib.pri)
