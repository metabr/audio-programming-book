# makefile for tdelay and vdelay
# to test psf_sndSeek etc
INCLUDES = -I./include
LIBS = -L./lib -lportsf -lm
PROGS= vdelay tdelay
CC= gcc

# CFLAGS = -I ./include -D_DEBUG -g
CFLAGS = -Dunix $(INCLUDES)
PSF = ./lib/libportsf.a
TDOBJS = tdelay.o fxdelay.o breakpoints.o
VDOBJS = vdelay.o wave.o fxdelay.o fxrand.o

.c.o:	gcc -c $(CFLAGS) $< -o $@ 

.PHONY:	clean veryclean

all:	$(PROGS)

./lib/libportsf.a: 
	cd portsf; make; make install

tdelay:	./lib/libportsf.a $(TDOBJS) 	
	$(CC) -o tdelay $(TDOBJS) $(PSF) $(LIBS)

vdelay:	./lib/libportsf.a $(VDOBJS) 
	$(CC) -o vdelay $(VDOBJS) $(PSF) $(LIBS)

clean:
	rm -f $(PROGS) $(TDOBJS) $(VDOBJS)

veryclean:	clean
	cd portsf; make veryclean


	
