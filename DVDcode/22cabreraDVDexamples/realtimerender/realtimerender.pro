SOURCES += main.cpp realtimerender.cpp
HEADERS += realtimerender.h
TEMPLATE = app
CONFIG += warn_on thread qt release
DESTDIR = ./
LIBS += -lportaudio
TARGET = realtimerender
