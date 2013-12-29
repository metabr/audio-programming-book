
INCLUDES 	= -I../../include
LIBS 		= ../../portsf/portsf.a

envx: envx.o $(LIBS)
	cc -o envx envx.o $(LIBS)
	
envx.o: envx.c 
	cc $(INCLUDES) -c -g envx.c
