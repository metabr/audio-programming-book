INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a
COMMON		= ../../common/

oscgen: library
	cc -o oscgen libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a breakpoints.o oscgen.o wave.o
	
compile: 
	cc $(INCLUDES) -c -g oscgen.c 
	cc $(INCLUDES) -c -g $(COMMON)breakpoints.c
	cc $(INCLUDES) -c -g $(COMMON)wave.c