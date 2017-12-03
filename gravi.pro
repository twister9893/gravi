#-------------------------------------------------
#
# Project created by QtCreator 2017-12-02T15:52:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gravi
TEMPLATE = app
LIBS += -lGLU

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    world.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    object.h \
    world.h

FORMS    += mainwindow.ui
