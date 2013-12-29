INCLUDES = -I./include
CFLAGS =  -O3 -Wall -Dunix $(INCLUDES) -Llib
CC=gcc
PORTSF = lib/libportsf.a
PROGS = bwfilter rbjfilter filtsf biquad

.PHONY:	portsf conv clean veryclean

all:	portsf biquad bwfilter rbjfilter filtsf

portsf:
	cd portsf; make; make install;

conv:
	cd conv; make;

biquad:	biquad.o ftools.o portsf
	$(CC) $(CFLAGS) -o biquad biquad.o ftools.o -lm $(PORTSF)

bwfilter:	bwfilter.o ftools.o breakpoints.o portsf
	$(CC) $(CFLAGS) -o bwfilter bwfilter.o ftools.o breakpoints.o -lm $(PORTSF)

filtsf:	filtsf.o ftools.o breakpoints.o portsf
	$(CC) $(CFLAGS) -o filtsf filtsf.o ftools.o breakpoints.o -lm $(PORTSF)

rbjfilter:	rbjfilter.o breakpoints.o ftools.o portsf
	$(CC) $(CFLAGS) -o rbjfilter rbjfilter.o ftools.o breakpoints.o -lm $(PORTSF)

clean: 
	rm -f *.o	
	rm -f $(PROGS)
	cd portsf; make clean
	cd conv; make clean

veryclean:
	cd portsf; make veryclean
