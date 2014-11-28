#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T22:28:19
#
#-------------------------------------------------

CONFIG += c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = quite
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        quca.hpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

QT += multimedia multimediawidgets
