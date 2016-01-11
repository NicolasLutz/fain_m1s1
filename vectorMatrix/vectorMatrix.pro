TARGET = vectorMatrix
TEMPLATE = lib
CONFIG += staticlib

INCLUDEPATH += ../

SOURCES += *.cpp \
    color.cpp \
    vec2f.cpp \
    vec3f.cpp \
    matrix33f.cpp \
    vec2i.cpp \
    polygon.cpp

HEADERS += *.h *.hpp \
    array.hpp \
    color.h \
    vector.hpp \
    vec2f.h \
    vec3f.h \
    matrix33f.h \
    common.h \
    vec2i.h \
    polygon.h \
    ../common.h

DESTDIR = ../lib

QMAKE_CXXFLAGS += -Wall

OTHER_FILES +=

