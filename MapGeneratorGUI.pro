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
    roadnetwork.cpp \
    designer.cpp \
    trafficlight.cpp \
    Graph/graph.cpp \
    Graph/vertex.cpp \
    Graph/edge.cpp \
    intersection.cpp \
    Cartographer/cartographer.cpp \
    lane.cpp

HEADERS  += mainwindow.h \
    road.h \
    utility.h \
    xmlparser.h \
    painter.h \
    mapgenerator.h \
    roadnetwork.h \
    designer.h \
    trafficlight.h \
    Graph/graph.h \
    Graph/vertex.h \
    Graph/edge.h \
    intersection.h \
    Cartographer/cartographer.h \
    lane.h

FORMS    += mainwindow.ui
