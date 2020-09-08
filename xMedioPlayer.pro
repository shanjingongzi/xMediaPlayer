QT       += core gui
QT       +=opengl
QT       +=multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audioplayer.cpp \
    avdecode.cpp \
    main.cpp \
    mainwindow.cpp \
    openglwidget.cpp \
    player.cpp

HEADERS += \
    audioplayer.h \
    avdecode.h \
    mainwindow.h \
    openglwidget.h \
    player.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH += $$PWD/../../../../usr/local/include/opencv4
INCLUDEPATH += $$PWD/../../../../usr/local/include
INCLUDEPATH +=/usr/include/x86_64-linux-gnu


unix:!macx: LIBS += -L$$PWD/ffmpeg/ -lavcodec

INCLUDEPATH += $$PWD/ffmpeg
DEPENDPATH += $$PWD/ffmpeg

unix:!macx: LIBS += -L$$PWD/ffmpeg/ -lavformat

INCLUDEPATH += $$PWD/ffmpeg
DEPENDPATH += $$PWD/ffmpeg

unix:!macx: LIBS += -L$$PWD/ffmpeg/ -lavfilter

INCLUDEPATH += $$PWD/ffmpeg
DEPENDPATH += $$PWD/ffmpeg

unix:!macx: LIBS += -L$$PWD/ffmpeg/ -lavutil

INCLUDEPATH += $$PWD/ffmpeg
DEPENDPATH += $$PWD/ffmpeg

unix:!macx: LIBS += -L$$PWD/ffmpeg/ -lswresample

INCLUDEPATH += $$PWD/ffmpeg
DEPENDPATH += $$PWD/ffmpeg


unix:!macx: LIBS += -L$$PWD/../../../../usr/local/lib/ -lopencv_world

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include
