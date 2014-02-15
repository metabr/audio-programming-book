INCLUDES = -I../portsf/include -I../TinyAudioLibrary -I./include
LIBS = -L../portsf/lib -L../TinyAudioLibrary -ltiny -lportsf -lm -lportaudio
PROGS = hellotable
PORTSF = ../portsf/lib/libportsf.a
TINYAUDIO = ../TinyAudioLibrary/libtiny.a
CC = gcc -Wall -std=c99

all: $(PROGS)

$(PORTSF):
	cd ../portsf/portsf; make; make install

$(TINYAUDIO):
	cd ../TinyAudioLibrary; make; make install

hellotable: hellotable.c $(TINYAUDIO)
	$(CC) -o hellotable hellotable.c $(INCLUDES) $(LIBS)

clean:
	rm -f $(PROGS)
	rm -f *.o

veryclean: clean
	cd /Users/br/src/portsf/portsf; make veryclean
