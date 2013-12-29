#   NB: we must compile cpp files with g++ rather than gcc, or we get obscure linker errors
#   because we are using exceptions; an alternative is to use -fexceptions
#
INCLUDES = -I../include
CC=gcc
CPP=g++ 
CFLAGS =  -O3 -Wall -Dunix $(INCLUDES) -L../lib
PORTSF = ../lib/libportsf.a
OBJS = vdelaypp.o vdelay.o vdelayb.o waveoscil.o

.c.o:
	$(CC) $(CFLAGS) -c $<
.cpp.o:
	$(CPP) $(CFLAGS) -c $<
#
#	targets
#
all:	vdelaypp

.PHONY:	clean veryclean $(PORTSF)

clean:
	-rm -f $(OBJS)
	rm -f vdelaypp 

veryclean:
	make clean; rm -f ../vdelaypp

vdelaypp:	$(OBJS) $(PORTSF)
	$(CPP) $(CFLAGS) $(OBJS) -o vdelaypp -lportsf -l stdc++
	cp vdelaypp ..

$(PORTSF): 	
	cd ../portsf; make; make install

#
#	dependencies
# NB: when classes etc are all defined in header files, it is especially important
# that these dependencies are declared. Otherwise, you can change a header file, type make
# and it will simply report that the program is up to date!
#
vdelaypp.o:  waveoscil.h oscil.h vdelay.h
vdelay.o:	vdelay.h
vdelayb.o:	vdelay.h
waveoscil.o:	waveoscil.h phasor.h

