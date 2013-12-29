#	C and C++ Programming for Musicians, MIT 2008
# 	DVD Chapter 5
#	makefile for miniplugin
#
#   NB: we must compile cpp files with g++ rather than gcc, or we get obscure linker errors
#   because we are using exceptions; an alternative is to use -fexceptions
#
CC=gcc
CPP=g++ 
CFLAGS =  -O3 -Wall -Dunix

OBJS =  buffer.o vdelayb.o miniplugin.o
.c.o:
	$(CC) $(CFLAGS) -c $<
.cpp.o:
	$(CPP) $(CFLAGS) -c $<
#
#	targets
#
all:	miniplugin

clean:
	rm -f $(OBJS)
	rm -f miniplugin

miniplugin:	$(OBJS) 
	$(CPP) $(CFLAGS) $(OBJS) -o miniplugin -l stdc++


#
#	dependencies
#
vdelay.cpp:	vdelayb.h
vdelayb.cpp:	vdelayb.h
miniplugin.cpp: miniplugin.h vdelayb.h 
