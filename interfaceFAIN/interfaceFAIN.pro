#-------------------------------------------------
#
# Project created by QtCreator 2016-01-04T22:49:23
#
#-------------------------------------------------

QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = interfaceFAIN
TEMPLATE = app

QMAKE_CXXFLAGS += -Wall

INCLUDEPATH += ../vectorMatrix ../image

LIBS +=  -L ../lib \
        -lvectorMatrix \
        -limage

PRE_TARGETDEPS = ../lib/*

DESTDIR = ../bin

SOURCES += main.cpp\
        mainwindow.cpp \
    renderimg.cpp

HEADERS  += mainwindow.h \
    renderimg.h

FORMS    += mainwindow.ui
