TARGET = Quite
VERSION = 0.1.0

CONFIG += c++11

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#quite_awesome.display_name = Quite
#DEPLOYMENT += quite_awesome
TEMPLATE = app


SOURCES  += main.cpp\
            main_window.cpp \
            image_manager.cpp\
            video_manager.cpp\
            quca.hpp\
            qweex/qweex_main_window.cpp\
            qweex/about_window.cpp


HEADERS  += project.h\
            main_window.h\
            media_manager.h\
            image_manager.h\
            video_manager.h\
            qweex/qweex_main_window.h\
            qweex/about_window.h


QT += multimedia multimediawidgets

OTHER_FILES += LICENSE*\
                ICONS.txt\
                resources/*.plist\
                resources/winicon.rc\
                resources/images/*\
                qweex/purchase/.keep

macx {
    ICON = resources/images/logo.icns
    CONFIG += static
    LIBS += -dead_strip

    ## For submission to app store ##
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
    QMAKE_INFO_PLIST = resources/Info.plist
}
win32 {
    ICON = resources/images/logo.ico
    LIBS += -lwinmm -lrpcrt4
    RC_FILE = resources/winicon.rc
}
linux-g++ {
}

# Proprietary stuff that I'm going to hell for
exists(qweex/purchase/purchase.h)
{
    DEFINES += "SUPPORT_THE_DEV"
    HEADERS += qweex/purchase/purchase.h qweex/purchase/qprogressindicator.h
    SOURCES += qweex/purchase/purchase.cpp qweex/purchase/qprogressindicator.cpp
    QT += network
}

RESOURCES += qweex/qweex.qrc

RESOURCES += resources/project.qrc
