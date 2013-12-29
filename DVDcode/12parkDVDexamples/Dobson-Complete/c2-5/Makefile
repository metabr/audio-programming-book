INCLUDES 	= -I../include
LIBS 		= ../portsf/portsf.a
COMMON		= ../common/

siggen: library
	cc -o siggen libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a breakpoints.o siggen.o wave.o
	
compile: 
	cc $(INCLUDES) -c -g siggen.c 
	cc $(INCLUDES) -c -g $(COMMON)breakpoints.c
	cc $(INCLUDES) -c -g $(COMMON)wave.c