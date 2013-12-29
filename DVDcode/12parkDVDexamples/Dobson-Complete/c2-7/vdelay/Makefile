INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a
COMMON		= ../../common/

vdelay: library
	cc -o vdelay libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a vdelay.o breakpoints.o fxdelay.o fxrand.o wave.o 
	
compile: 
	cc $(INCLUDES) -c -g vdelay.c 
	cc $(INCLUDES) -c -g $(COMMON)breakpoints.c
	cc $(INCLUDES) -c -g $(COMMON)fxdelay.c
	cc $(INCLUDES) -c -g $(COMMON)fxrand.c
	cc $(INCLUDES) -c -g $(COMMON)wave.c