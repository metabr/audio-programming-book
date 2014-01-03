INCLUDES = -I./include
LIBS = -L./lib -lportsf -lm
PROGS = sf2float
PORTSF = ./lib/libportsf.a
CC = gcc

all: $(PROGS)

$(PORTSF):
	cd portsf; make; make install

sf2float: sf2float.c $(PORTSF)
	$(CC) -o sf2float sf2float.c $(INCLUDES) $(LIBS)

clean:
	rm -f $(PROGS)
	rm -f *.o

veryclean: clean
	cd portsf; make veryclean
