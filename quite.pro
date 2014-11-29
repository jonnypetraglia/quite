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
        quca.hpp\
        image_manager.cpp\
        video_manager.cpp

HEADERS  += mainwindow.h\
            media_manager.h\
            image_manager.h\
            video_manager.h

FORMS    += mainwindow.ui

QT += multimedia multimediawidgets
