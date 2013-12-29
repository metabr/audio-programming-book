INCLUDES 	= -I../include
# NB portsf not used for test programs
# with the odd exception, these programs can be built without a makefile, especially on OS X

PROGS = buftest constcast defargs delaytest dyncast etscale exception memtestdjd runerr sfx \
  tempfib testcomplex testcon testmin tststrm testtbuf
CC = gcc
CPP = g++
CFLAGS = -O2 -Wall -Dunix $(INCLUDES)


.c.o:
	$(CC) $(CFLAGS) -c $<
.cpp.o:
	$(CPP) $(CFLAGS) -c $<

all: $(PROGS)

vdelay.o:	../vdelaycpp/vdelay.cpp
	$(CPP) $(CFLAGS) -o vdelay.o -c ../vdelaycpp/vdelay.cpp

buftest:	buftest.o vdelay.o
	$(CPP) $(CFLAGS) -o buftest buftest.o vdelay.o -lstdc++

constcast:	constcast.o
	$(CPP) $(CFLAGS) -o constcast constcast.o  -lstdc++	

delay.o:	../vdelaycpp/delay.cpp
	$(CPP) $(CFLAGS) -o delay.o -c ../vdelaycpp/delay.cpp

defargs:	defargs.o
	$(CPP) $(CFLAGS) -o defargs defargs.o vdelay.o -lstdc++

delaytest:	delaytest.o delay.o
	$(CPP) $(CFLAGS) -o delaytest delay.o delaytest.o  -lstdc++

dyncast:	dyncast.o
	$(CPP) $(CFLAGS) -o dyncast dyncast.o  -lstdc++

etscale:	etscale.o
	$(CPP) $(CFLAGS) -o etscale etscale.o  -lstdc++

exception:	exception.o
	$(CPP) $(CFLAGS) -o exception exception.o vdelay.o -lstdc++

memtestdjd:	memtestdjd.o
	$(CPP) $(CFLAGS) -o memtestdjd memtestdjd.o  -lstdc++

runerr:	runerr.o
	$(CPP) $(CFLAGS) -o runerr runerr.o  -lstdc++

sfx:	sfx.o
	$(CPP) $(CFLAGS) -o sfx sfx.o vdelay.o -lstdc++

tempfib:	tempfib.o
	$(CPP) $(CFLAGS) -o tempfib tempfib.o  -lstdc++

testcomplex:	testcomplex.o
	$(CPP) $(CFLAGS) -o testcomplex testcomplex.o  -lstdc++

testcon:	testcon.o
	$(CPP) $(CFLAGS) -o testcon testcon.o  -lstdc++

testmin:	testmin.o
	$(CPP) $(CFLAGS) -o testmin testmin.o  -lstdc++

tststrm:	tststrm.o
	$(CPP) $(CFLAGS) -o tststrm tststrm.o  -lstdc++

testtbuf:	testtbuf.o
	$(CPP) $(CFLAGS) -o testtbuf testtbuf.o  -lstdc++

clean:
	rm -f $(PROGS)
	rm *.o




#
#dependencies
#

delay.o:	../vdelaycpp/delay.h
delaytest.o:	../vdelaycpp/delay.h
testtbuf.o:	tbuffer.h

