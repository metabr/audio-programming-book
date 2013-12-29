SOURCES += main.cpp offlinerender.cpp ../lazzarini/soundio.cpp ../lazzarini/delay.cpp
HEADERS += offlinerender.h
TEMPLATE = app
CONFIG += warn_on thread qt release
DESTDIR = ./
INCLUDEPATH += "../lazzarini"
LIBS += -lsndfile
TARGET = offlinerender
