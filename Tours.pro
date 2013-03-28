#-------------------------------------------------
#
# Project created by QtCreator 2013-03-27T17:48:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Tours
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    board.cpp \
    square.cpp \
    path.cpp

HEADERS  += mainwindow.h \
    Common.h \
    board.h \
    square.h \
    path.h

FORMS    += mainwindow.ui
