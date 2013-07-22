#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T16:16:21
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QOggSimplePlayer
TEMPLATE = app

LIBS += -lvorbis -lvorbisfile

SOURCES += main.cpp\
        MainWindow.cpp \
    QOggAudioBuffer.cpp \
    QOggSimplePlayer.cpp

HEADERS  += MainWindow.h \
    QOggAudioBuffer.h \
    QOggSimplePlayer.h

FORMS    += MainWindow.ui
