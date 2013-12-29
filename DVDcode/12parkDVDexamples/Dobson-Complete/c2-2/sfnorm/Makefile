INCLUDES 	= -I../../include
LIBS 		=../../portsf/portsf.a

sfnorm: sfnorm.o $(LIBS)
	cc -o sfnorm sfnorm.o $(LIBS)
	
sfnorm.o: sfnorm.c 
	cc $(INCLUDES) -c -g sfnorm.c