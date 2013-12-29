# OS X makefile for sfplay ; audio file player using Portaudio v 19

INCLUDES = -I../../include
LIBS = -L../../lib -lportsf  -lportaudio -lm
CC = gcc
OPT = -O3
#OPT = -g
#CFLAGS = $(OPT) -Dunix -DMAC -D_DEBUG -DPA_USE_COREAUDIO 
CFLAGS = $(OPT) -Dunix -DMAC  -DPA_USE_COREAUDIO   -I../include
IOLINK =  -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework Carbon
   
PORTSF = ../lib/libportsf.a 

.c.o:
	$(CC) $(CFLAGS) -c $<
	 
sfplay: sfplay.o $(PORTSF) 
	$(CC) $(CFLAGS) sfplay.o  -o sfplay $(IOLINK) $(PORTSF)  -lm -lportaudio

../lib/libportsf.a:
	cd ../portsf; make; make install

clean: 
	rm -f $(PAXCORE) sfplay.o
	rm -f sfplay





