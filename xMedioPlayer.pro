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
    avdecode.cpp \
    main.cpp \
    mainwindow.cpp \
    openglwidget.cpp

HEADERS += \
    avdecode.h \
    mainwindow.h \
    openglwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
QMAKE_LIBDIR+=/opt/Qt5.14.2/5.14.2/gcc_64/lib
INCLUDEPATH += $$PWD/../../../../usr/local/include/opencv4
INCLUDEPATH += $$PWD/../../../../usr/local/include
QMAKE_LIBDIR+= /usr/local/lib
DEPENDPATH+=/opt/Qt5.14.2/5.14.2/gcc_64/lib
unix:!macx: LIBS += -lavcodec
unix:!macx: LIBS += -lavformat
unix:!macx: LIBS += -lavutil
unix:!macx: LIBS += -lavfilter
unix|win32: LIBS += -lopencv_world

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/x86_64-linux-gnu/ -lswresample

INCLUDEPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../usr/lib/x86_64-linux-gnu
