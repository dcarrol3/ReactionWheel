#-------------------------------------------------
#
# Project created by QtCreator 2016-09-11T20:03:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReactionWheel
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    reactionwheelplatform.cpp

HEADERS  += mainwindow.h \
    serial.h \
    reactionwheelplatform.hpp\
    json.hpp

FORMS    += mainwindow.ui

CONFIG += c++11

LIBS += \
       -lboost_system\
