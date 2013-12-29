
INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a

sfgain: sfgain.o $(LIBS)
	cc -o sfgain sfgain.o $(LIBS)
	
sfgain.o: sfgain.c 
	cc $(INCLUDES) -c -g sfgain.c
