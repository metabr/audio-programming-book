INCLUDES 	= -I../include
LIBS 		= ../portsf/portsf.a

sf2float: sf2float.o $(LIBS)
	cc -o sf2float sf2float.o $(LIBS)
	
sf2float.o: sf2float.c 
	cc $(INCLUDES) -c -g sf2float.c