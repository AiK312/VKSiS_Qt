QT += core
QT -= gui

TARGET = VKSiS_Qt
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    PCX.cpp

HEADERS += \
    pcx.h \
    ImgUtils.hpp

