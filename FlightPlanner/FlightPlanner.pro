#-------------------------------------------------
#
# Project created by QtCreator 2016-10-03T14:32:37
#
#-------------------------------------------------

QT       += core gui\
            serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FlightPlanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mywindow.cpp \
    secondwindow.cpp \
    UART.cpp \
    sendcompleted.cpp

HEADERS  += mainwindow.h \
    mywindow.h \
    secondwindow.h \
    UART.h \
    sendcompleted.h

FORMS    += mainwindow.ui \
    secondwindow.ui \
    sendcompleted.ui
