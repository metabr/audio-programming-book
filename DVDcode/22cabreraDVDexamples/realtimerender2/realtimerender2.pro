SOURCES += main.cpp realtimerender2.cpp ../realtimerender/realtimerender.cpp
HEADERS += realtimerender2.h ../realtimerender/realtimerender.h
TEMPLATE = app
CONFIG += warn_on thread  qt release
DESTDIR = ./
LIBS += -lportaudio
TARGET = realtimerender2
