# fake320

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fake320
TEMPLATE = app

CONFIG += release

QMAKE_CXXFLAGS += -std=c++11

win32
{
    INCLUDEPATH += c:/mingw32/include
    LIBS += -static -Lc:/mingw32/lib -lavformat -lavfilter -lavcodec -lavutil  -lfftw3
}


HEADERS += \
    src/mainwindow.h \
    src/mp3file.h \
    src/mp3library.h

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mp3file.cpp \
    src/mp3library.cpp
