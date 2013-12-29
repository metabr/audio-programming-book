INCLUDES 	= -I../include
LIBS 		= ../portsf/portsf.a
COMMON		= ../common/

sfpan: library
	cc -o sfpan libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a breakpoints.o sfpan.o
	
compile:  
	cc $(INCLUDES) -c -g sfpan.c 
	cc $(INCLUDES) -c -g $(COMMON)breakpoints.c




 

	