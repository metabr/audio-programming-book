INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a
COMMON		= ../../common/

bwfilter: library
	cc -o bwfilter libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a ftools.o bwfilter.o breakpoints.o
	
compile: 
	cc $(INCLUDES) -c -g bwfilter.c 
	cc $(INCLUDES) -c -g $(COMMON)ftools.c $(COMMON)breakpoints.c

