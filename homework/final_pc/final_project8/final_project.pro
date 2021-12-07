#-------------------------------------------------
#
# Project created by QtCreator 2020-10-31T19:02:55
#
#-------------------------------------------------

QT       += core gui
QT += multimedia
QT+=multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = final_project
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    object.cpp \
    wall.cpp \
    fieldfunction.cpp \
    player.cpp \
    block.cpp \
    bomb.cpp \
    props.cpp

HEADERS  += mainwindow.h \
    object.h \
    wall.h \
    fieldfunction.h \
    player.h \
    block.h \
    bomb.h \
    props.h

FORMS    += mainwindow.ui

RESOURCES += \
    picture.qrc \
    sound.qrc

