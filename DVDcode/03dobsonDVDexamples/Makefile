INCLUDES 	= -I./include
LIBS 		= -L./lib -lportsf -lm
PROGS = tabgen2 bitwise varargdemo
CC = gcc
CFLAGS = -O2

all: $(PROGS)

.PHONY:	portsf clean veryclean

portsf: 
	cd portsf; make; make install

bitwise:	bitwise.c
	$(CC)  -o bitwise bitwise.c 

varargdemo:	varargdemo.c
	$(CC) -o varargdemo varargdemo.c

tabgen2: tabgen2.c gtable2.c breakpoints.c portsf
	$(CC)  $(CFLAGS) -o tabgen2 tabgen2.c gtable2.c breakpoints.c $(INCLUDES) $(LIBS)

clean:
	rm -f $(PROGS)
	rm -f *.o

veryclean:	clean
	cd portsf; make veryclean
