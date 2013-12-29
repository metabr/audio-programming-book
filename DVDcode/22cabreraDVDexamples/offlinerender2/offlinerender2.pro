SOURCES += main.cpp \
 ../lazzarini/soundio.cpp \
 ../lazzarini/delay.cpp \
 offlinerender2.cpp \
 ../offlinerender/offlinerender.cpp
HEADERS += offlinerender2.h ../offlinerender/offlinerender.h
TEMPLATE = app
CONFIG += warn_on thread qt release
DESTDIR = ./
INCLUDEPATH += "../lazzarini"
LIBS += -lsndfile
TARGET = offlinerender2
