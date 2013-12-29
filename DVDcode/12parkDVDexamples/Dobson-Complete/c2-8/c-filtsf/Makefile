INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a
COMMON		= ../../common/

filtsf: library
	cc -o filtsf libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a ftools.o filtsf.o breakpoints.o
	
compile: 
	cc $(INCLUDES) -c -g filtsf.c 
	cc $(INCLUDES) -c -g $(COMMON)ftools.c $(COMMON)breakpoints.c

