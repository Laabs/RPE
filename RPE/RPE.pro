#-------------------------------------------------
#
# Project created by QtCreator 2019-11-07T14:49:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RPE
TEMPLATE = app


SOURCES += main.cpp\
        choosewindow.cpp \
        mainwindow.cpp \
        plotwindow1.cpp \
        subdialog.cpp

HEADERS  += mainwindow.h \
    choosewindow.h \
    plotwindow1.h \
    subdialog.h

FORMS    += mainwindow.ui \
    choosewindow.ui \
    plotwindow1.ui \
    subdialog.ui

RESOURCES += \
    res.qrc

DISTFILES += UNICODE
