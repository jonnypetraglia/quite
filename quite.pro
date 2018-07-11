TARGET = Quite
VERSION = 0.1.0

CONFIG += c++11 avwidgets
#av

QT += core gui av opengl avwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app


SOURCES  += main.cpp\
            main_window.cpp \
            image_manager.cpp\
            video_manager.cpp\
            quca.hpp


HEADERS  += project.h\
            main_window.h\
            media_manager.h\
            image_manager.h\
            video_manager.h \
            platform/mac.h


QT += multimedia multimediawidgets

OTHER_FILES += README.md\
                LICENSE\
                ICONS.txt\
                platform/*\
                images/*

mac {
    ICON = images/logo.icns
    CONFIG += static
    LIBS += -dead_strip

    ## For submission to app store ##
    #QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
    #QMAKE_MAC_SDK = macosx
    #QMAKE_INFO_PLIST = platform/Info.plist

    OBJECTIVE_SOURCES += platform/mac.mm
    LIBS += -framework Cocoa

    QMAKE_RPATHDIR=$QTDIR/lib

}
win32 {
    ICON = images/logo.ico
    LIBS += -lwinmm -lrpcrt4
    RC_FILE = platform/winicon.rc
}
linux-g++ {
}

RESOURCES += project.qrc

include(qwmainwindow/qwmainwindow.pri)
