#-------------------------------------------------
#
# Project created by QtCreator 2014-03-28T18:03:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Fake320
TEMPLATE = app


SOURCES += ../../src/main.cpp\
        ../../src/mainwindow.cpp\
        ../../src/mp3file.cpp\
        ../../src/mp3library.cpp\
        ../../src/itemmodel.cpp

HEADERS  += ../../src/mainwindow.h\
            ../../src/mp3file.h\
            ../../src/mp3library.h\
            ../../src/itemmodel.h
            

INCLUDEPATH += /usr/local/include

macx: LIBS += -L$$PWD/../../lib/ -lavformat -lavcodec -lavutil -lfftw3

INCLUDEPATH += $$PWD/../../
DEPENDPATH += $$PWD/../../

macx: PRE_TARGETDEPS += $$PWD/../../lib/libavformat.a\
                        $$PWD/../../lib/libavcodec.a\
                        $$PWD/../../lib/libavutil.a\
                        $$PWD/../../lib/libfftw3.a


