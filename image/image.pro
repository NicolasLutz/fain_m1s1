TARGET = image
TEMPLATE = lib
CONFIG += staticlib

QT += opengl

INCLUDEPATH += ../vectorMatrix

SOURCES += *.cpp \
    image2grey.cpp \
    image2color.cpp \
    imgGradient.cpp

HEADERS += *.h *.hpp \
    imgGradient.h \
    image2grey.h \
    image2d.hpp \
    image2color.h \
    Iimage.hpp

DESTDIR = ../lib
