#	makefile for conv
#
#   NB: we must compile cpp files with g++ rather than gcc, or we get obscure linker errors
#   because we are using exceptions; an alternative is to use -fexceptions
#
INCLUDES = -I../include
CC=gcc
CPP=g++ 
CFLAGS =  -O3 -Wall -Dunix $(INCLUDES) -L../lib
PORTSF = ../lib/libportsf.a
OBJS = conv.o fconv.o

.c.o:
	$(CC) $(CFLAGS) -c $<
.cpp.o:
	$(CPP) $(CFLAGS) -c $<
#
#	targets
#
.PHONY:	portsf

all:	conv

clean:
	-rm -f $(OBJS)
	rm -f conv

conv:	$(OBJS) $(PORTSF)
	$(CC) $(CFLAGS) $(OBJS) -o conv $(PORTSF) -lm


$(PORTSF):	
	cd ../portsf; make; make install

#
#	dependencies
#

fconv.c,conv.c:	fconv.h

