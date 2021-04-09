#-------------------------------------------------
#
# Project created by QtCreator 2021-03-29T19:41:12
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = task2
TEMPLATE = app

FORMS += \
    Views/mainwindow.ui \
    Views/signalview.ui \
    Views/generalview.ui \
    Views/settingswindow.ui \
    Views/simplesignalview.ui

HEADERS += \
    Buiseness/signalgenerator.h \
    Views/mainwindow.h \
    Views/qcustomplot.h \
    Views/signalview.h \
    Views/generalview.h \
    Views/settingswindow.h \
    Views/simplesignalview.h

SOURCES += \
    Buiseness/main.cpp \
    Buiseness/signalgenerator.cpp \
    Views/mainwindow.cpp \
    Views/qcustomplot.cpp \
    Views/signalview.cpp \
    Views/generalview.cpp \
    Views/settingswindow.cpp \
    Views/simplesignalview.cpp

RESOURCES += \
    Resources/resources.qrc

DISTFILES += \
    Resources/onOffIcon.png
