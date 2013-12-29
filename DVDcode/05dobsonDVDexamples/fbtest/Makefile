#	makefile for siggenpp
#
#   NB: we must compile cpp files with g++ rather than gcc, or we get obscure linker errors
#   because we are using exceptions; an alternative is to use -fexceptions
#
INCLUDES = -I../include -I/usr/include
CC=gcc
CPP=g++ 
CFLAGS =  -O3 -Wall -Dunix $(INCLUDES) -L../lib  -L/usr/local/lib
PORTSF = ../lib/libportsf.a
OBJS = fbtest.o ftools.o breakpoints.o

.c.o:
	$(CC) $(CFLAGS) -c $<
.cpp.o:
	$(CPP) $(CFLAGS) -c $<
#
#	targets
#
.PHONY:	clean $(PORTSF)

all:	fbtest

clean:
	rm -f $(OBJS)
	rm -f fbtest 

fbtest:	$(OBJS) $(PORTSF)
	$(CPP) $(CFLAGS) $(OBJS) -o fbtest $(PORTSF) -lm
	
$(PORTSF):	
	cd ../portsf; make; make install

#
#	dependencies
#
