#-------------------------------------------------
#
# Project created by QtCreator 2014-03-28T18:03:18
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fake320
TEMPLATE = app

QMAKE_CXXFLAGS += -mmacosx-version-min=10.7 -std=c++11 -stdlib=libc++
CONFIG += c++11

HEADERS  += src/mainwindow.h src/mp3file.h src/mp3library.h
SOURCES += src/main.cpp src/mainwindow.cpp src/mp3file.cpp src/mp3library.cpp

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lavformat -lavcodec -lavutil -lfftw3

