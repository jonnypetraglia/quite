#-------------------------------------------------
#
# Project created by QtCreator 2013-07-17T22:28:19
#
#-------------------------------------------------

CONFIG += c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Quite
TEMPLATE = app


SOURCES  += main.cpp\
            main_window.cpp\
            quca.hpp\
            image_manager.cpp\
            video_manager.cpp

HEADERS  += main_window.h\
            media_manager.h\
            image_manager.h\
            video_manager.h

FORMS    +=

QT += multimedia multimediawidgets

OTHER_FILES += Todo.txt
