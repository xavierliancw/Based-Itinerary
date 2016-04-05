#-------------------------------------------------
#
# Project created by QtCreator 2016-03-22T21:58:43
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Baseball-Itinerary-Program
TEMPLATE = app


SOURCES += main.cpp\
        primewin.cpp \
    adminlogin.cpp \
    datastructures.cpp

HEADERS  += primewin.h \
    adminlogin.h \
    datastructures.h

FORMS    += primewin.ui \
    adminlogin.ui
