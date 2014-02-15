#
#	makefile for Tiny
#
#
#	VL 2009
#
CC=gcc

CFLAGS=-O2 -Wall -I/usr/local/include

PSFOBJS=tinyAudioLib.o

.c.o:
	$(CC) $(CFLAGS) -c $<
#
#	targets
#
all:	libtiny.a

libtiny.a:	$(PSFOBJS)
	ar rc libtiny.a $(PSFOBJS)
install:	libtiny.a
	#cp libtiny.a ../lib/libtiny.a
	ranlib libtiny.a

#
#	dependencies
#
tinyAudioLib.o:	tinyAudioLib.h

clean:
	rm -f *.a
	rm -f *.o
