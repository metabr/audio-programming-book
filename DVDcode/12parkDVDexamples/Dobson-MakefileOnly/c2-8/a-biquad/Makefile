INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a
COMMON		= ../../common/

biquad: library
	cc -o biquad libportsf.a $(LIBS)

library: compile
	ar rvs libportsf.a ftools.o biquad.o
	
compile: 
	cc $(INCLUDES) -c -g biquad.c 
	cc $(INCLUDES) -c -g $(COMMON)ftools.c
