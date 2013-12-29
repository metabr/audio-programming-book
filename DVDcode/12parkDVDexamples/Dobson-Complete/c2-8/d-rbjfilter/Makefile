INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a
COMMON		= ../../common/

rbjfilter: library
	cc -o rbjfilter libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a breakpoints.o rbjfilter.o cfsmp.o ftools.o
	
compile: 
	cc $(INCLUDES) -c -g rbjfilter.c 
	cc $(INCLUDES) -c -g $(COMMON)breakpoints.c $(COMMON)ftools.c
	cc $(INCLUDES) -c -g cfsmp.c