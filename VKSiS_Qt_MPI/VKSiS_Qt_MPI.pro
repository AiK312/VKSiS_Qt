QT += core
QT -= gui

TARGET = VKSiS_Qt_MPI
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

INCLUDEPATH += "C:\Program Files (x86)\Microsoft SDKs\MPI\Include"
LIBS += "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86\msmpi.lib"
LIBS += "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86\msmpifec.lib"
LIBS += "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86\msmpifes.lib"
LIBS += "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86\msmpifmc.lib"
LIBS += "C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86\msmpifms.lib"


TEMPLATE = app

SOURCES += main.cpp

