FORMS += matrixmode.ui \
    mainwindow.ui \
    rawmode.ui
HEADERS += porttime.h \
    portmidi.h \
    morecond.h \
    mainwindow.h \
    editwindow.h \
    define.h \
    cond.h \
    matrixmode.h \
    rawmode.h
SOURCES += mainwindow.cpp \
    main.cpp \
    editwindow.cpp \
    matrixmode.cpp \
    rawmode.cpp
LIBS += -lportmidi
