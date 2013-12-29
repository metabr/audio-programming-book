#-------------------------------------------------
#
# Project created by QtCreator 2010-07-11T20:09:30
#
#-------------------------------------------------

TARGET = MIDI_Monitor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS    += -lportmidi
