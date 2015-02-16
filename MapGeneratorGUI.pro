#-------------------------------------------------
#
# Project created by QtCreator 2015-02-05T09:54:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapGeneratorGUI

#CONFIG += console
CONFIG += c++11

TEMPLATE = app



SOURCES += main.cpp\
        mainwindow.cpp \
    road.cpp \
    xmlparser.cpp \
    painter.cpp \
    mapgenerator.cpp \
    roadnetwork.cpp

HEADERS  += mainwindow.h \
    road.h \
    utility.h \
    xmlparser.h \
    painter.h \
    mapgenerator.h \
    roadnetwork.h

FORMS    += mainwindow.ui
