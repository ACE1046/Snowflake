#-------------------------------------------------
#
# Project created by QtCreator 2021-12-11T18:46:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTStars
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hsv2rgb.cpp \
    effects.cpp

HEADERS  += mainwindow.h \
    effects.h

FORMS    += mainwindow.ui
