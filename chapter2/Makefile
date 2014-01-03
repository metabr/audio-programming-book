INCLUDES = -I/Users/br/src/portsf/include
LIBS = -L/Users/br/src/portsf/lib -lportsf -lm
PROGS = sf2float sf2float-loop sfprop
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

clean:
	rm -f $(PROGS)
	rm -f *.o

veryclean: clean
	cd /Users/br/src/portsf/portsf; make veryclean
