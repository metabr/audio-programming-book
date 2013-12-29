INCLUDES 	= -I../include

portsf.a: portsf.c ieee80.c ieee80.h 
	cc $(INCLUDES) -c -g portsf.c
	cc -c ieee80.c
	ar rvs portsf.a portsf.o ieee80.o
	rm portsf.o ieee80.o


