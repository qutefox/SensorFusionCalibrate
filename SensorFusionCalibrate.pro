QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application.cpp \
    dataitem.cpp \
    main.cpp \
    mainwindow.cpp \
    math/matrices.cpp \
    math/matricies_tests.cpp \
    terminal/escape/escape_osc.cpp \
    terminal/escape/terminal_data.cpp \
    terminal/escape/terminal_escape.cpp \
    terminal/terminal.cpp

HEADERS += \
    application.h \
    dataitem.h \
    mainwindow.h \
    math/matrices.h \
    terminal/escape/escape_osc.h \
    terminal/escape/terminal_data.h \
    terminal/escape/terminal_escape.h \
    terminal/terminal.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
