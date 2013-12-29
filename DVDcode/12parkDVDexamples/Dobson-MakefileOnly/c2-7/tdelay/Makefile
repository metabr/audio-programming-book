INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a
COMMON		= ../../common/

tdelay: library
	cc -o tdelay libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a breakpoints.o tdelay.o fxdelay.o
	
compile: 
	cc $(INCLUDES) -c -g tdelay.c 
	cc $(INCLUDES) -c -g $(COMMON)breakpoints.c
	cc $(INCLUDES) -c -g $(COMMON)fxdelay.c