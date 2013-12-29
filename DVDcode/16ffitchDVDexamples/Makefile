HDRS = midilib.h midilib-i.h
SRC = basic.c
OBJ = basic.o

CC = gcc
CFLAGS = -Wall -O2 -g
AR = ar
RANLIB = echo

all:	midilib.a midimono miditest midiex midiex1

midilib.a:	${OBJ}
	${AR} -crs midilib.a ${OBJ}
	${RANLIB} midilib.a

.c.o:	${HDR}
	${CC} -c ${CFLAGS} $*.c -I/usr/include

midiex: midiexamp.c midilib.a midilib.h
	${CC} -o midiex midiexamp.c midilib.a

midiex1: midiexamp1.c midilib.a midilib.h
	${CC} -o midiex1 midiexamp1.c midilib.a

midimono: midimono.c midilib.a midilib.h
	${CC} -o midimono midimono.c midilib.a -I.

miditest: miditest.c midilib.a midilib.h
	${CC} -o miditest miditest.c midilib.a -I.
