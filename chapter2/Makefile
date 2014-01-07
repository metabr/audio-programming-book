INCLUDES = -I/Users/br/src/portsf/include -I./include
LIBS = -L/Users/br/src/portsf/lib -lportsf -lm
PROGS = sf2float sf2float-loop sfprop sfgain sfnorm sfpeak sfpan
PORTSF = /Users/br/src/portsf/lib/libportsf.a
CC = gcc

all: $(PROGS)

$(PORTSF):
	cd /Users/br/src/portsf/portsf; make; make install

sf2float: sf2float.c $(PORTSF)
	$(CC) -o sf2float sf2float.c $(INCLUDES) $(LIBS)

sf2float-loop: sf2float-loop.c $(PORTSF)
	$(CC) -o sf2float-loop sf2float-loop.c $(INCLUDES) $(LIBS)

sfprop: sfprop.c $(PORTSF)
	$(CC) -o sfprop sfprop.c $(INCLUDES) $(LIBS)

sfgain: sfgain.c $(PORTSF)
	$(CC) -o sfgain sfgain.c $(INCLUDES) $(LIBS)

sfnorm: sfnorm.c $(PORTSF)
	$(CC) -o sfnorm sfnorm.c $(INCLUDES) $(LIBS)

sfpeak: sfpeak.c $(PORTSF)
	$(CC) -o sfpeak sfpeak.c $(INCLUDES) $(LIBS)

sfpan: sfpan.c breakpoints.c $(PORTSF)
	$(CC) -o sfpan sfpan.c breakpoints.c $(INCLUDES) $(LIBS)

clean:
	rm -f $(PROGS)
	rm -f *.o

veryclean: clean
	cd /Users/br/src/portsf/portsf; make veryclean
