#-------------------------------------------------
#
# Project created by QtCreator 2015-10-16T23:29:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SpaceAlien
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    game.cpp \
    player.cpp \
    enemyspawn.cpp \
    alien.cpp \
    boss.cpp


HEADERS  += mainwindow.h \
    game.h \
    player.h \
    enemyspawn.h \
    alien.h \
    boss.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11

RESOURCES += \
    resources.qrc
QT += multimedia
