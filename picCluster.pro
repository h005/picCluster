#-------------------------------------------------
#
# Project created by QtCreator 2015-12-10T06:53:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = picCluster
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imglabel.cpp \
    cluster.cpp \
    treecluster.cpp

HEADERS  += mainwindow.h \
    imglabel.h \
    cluster.h \
    mode.h \
    treecluster.h

FORMS    += mainwindow.ui

INCLUDEPATH += D:\opencv\include\opencv\
                D:\opencv\include\opencv2\
                D:\opencv\include\

LIBS += D:/opencv/lib/*.a
