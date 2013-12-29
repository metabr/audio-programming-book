#	makefile for siggenpp
#
#   NB: we must compile cpp files with g++ rather than gcc, or we get obscure linker errors
#   because we are using exceptions; an alternative is to use -fexceptions
#
INCLUDES = -I../include
CC=gcc
CPP=g++ 
CFLAGS =  -O3 -Wall -Dunix $(INCLUDES) -L../lib
PORTSF = ../lib/libportsf.a
OBJS = siggen.o waveoscil.o BreakStream.o
.c.o:
	$(CC) $(CFLAGS) -c $<
.cpp.o:
	$(CPP) $(CFLAGS) -c $<
#
#	targets
#

.PHONY:	clean veryclean portsf

all:	siggenpp

clean:
	-rm -f $(OBJS)
	rm -f siggenpp 

veryclean:
	make clean;
	rm -f ../siggenpp

siggenpp:	$(OBJS) portsf
	$(CPP) $(CFLAGS) $(OBJS) -o siggenpp $(PORTSF) -l stdc++
	cp siggenpp ..

portsf:
	cd ../portsf; make; make install

#
#	dependencies
#
#  See text. choice of oscil.h or waveoscil.h  for siggenpp
# NB: where classes are fully defined in header files it is especially important
# to declare them as dependencies. otherwise, editing them will not be noticed my make.
siggen.o:	waveoscil.h BreakStream.h