INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a
COMMON		= ../../common/

tabgen: library
	cc -o tabgen libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a breakpoints.o tabgen.o gtable.o
	
compile: 
	cc $(INCLUDES) -c -g tabgen.c 
	cc $(INCLUDES) -c -g $(COMMON)breakpoints.c
	cc $(INCLUDES) -c -g gtable.c