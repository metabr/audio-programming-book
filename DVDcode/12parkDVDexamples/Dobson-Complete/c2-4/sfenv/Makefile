INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a
COMMON		= ../../common/

sfenv: library
	cc -o sfenv libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a breakpoints.o sfenv.o
	
compile: 
	cc $(INCLUDES) -c -g sfenv.c 
	cc $(INCLUDES) -c -g $(COMMON)breakpoints.c
