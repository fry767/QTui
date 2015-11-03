#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T22:08:12
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestUi
TEMPLATE = app


SOURCES += \
    main.cpp\
    mainwindow.cpp \
    serialport.cpp \
    bsp.cpp \
    settingsdialog.cpp \
    console.cpp

HEADERS  += mainwindow.h \
    serialport.h \
    bsp.h \
    settingsdialog.h \
    console.h

FORMS    += mainwindow.ui \
    settingsdialog.ui
