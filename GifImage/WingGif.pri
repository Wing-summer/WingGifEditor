SOURCES += \
    $$PWD/quantizer/KMeansQuantizer.cpp \
    $$PWD/quantizer/MedianCutQuantizer.cpp \
    $$PWD/quantizer/NeuQuantQuantizer.cpp \
    $$PWD/quantizer/OctreeQuantizer.cpp \
    $$PWD/quantizer/RandomQuantizer.cpp \
    $$PWD/quantizer/UniformQuantizer.cpp \
    $$PWD/quantizer/KDTree.cpp \
    $$PWD/quantizer/NeuQuant.cpp \
    $$PWD/encoder/GifEncoder.cpp \
    $$PWD/encoder/GifBlockWriter.cpp \
    $$PWD/encoder/BayerDitherer.cpp \
    $$PWD/encoder/FloydSteinbergDitherer.cpp \
    $$PWD/encoder/LzwEncoder.cpp \
    $$PWD/encoder/M2Ditherer.cpp \
    $$PWD/encoder/NoDitherer.cpp \
    $$PWD/decoder/gifdecoder.cpp

HEADERS += \
    $$PWD/quantizer/KMeansQuantizer.h \
    $$PWD/quantizer/MedianCutQuantizer.h \
    $$PWD/quantizer/NeuQuantQuantizer.h \
    $$PWD/quantizer/OctreeQuantizer.h \
    $$PWD/quantizer/RandomQuantizer.h \
    $$PWD/quantizer/UniformQuantizer.h \
    $$PWD/quantizer/ColorQuantizer.h \
    $$PWD/ThreadPool.h \
    $$PWD/quantizer/KDTree.h \
    $$PWD/quantizer/NeuQuant.h \
    $$PWD/encoder/GifEncoder.h \
    $$PWD/encoder/GifBlockWriter.h \
    $$PWD/encoder/BayerDitherer.h \
    $$PWD/encoder/FloydSteinbergDitherer.h \
    $$PWD/encoder/LzwEncoder.h \
    $$PWD/encoder/M2Ditherer.h \
    $$PWD/encoder/Ditherer.h \
    $$PWD/encoder/NoDitherer.h \
    $$PWD/decoder/gifdecoder.h

CONFIG += c++14

QMAKE_CXXFLAGS += -std=c++11

include($$PWD/decoder/giflib.pri)
