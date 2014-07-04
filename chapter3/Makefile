INCLUDES = -I../portsf/include -I../TinyAudioLibrary -I./include
LIBS = -lm
LPORTSF = -L../portsf/lib -lportsf
LTINYAUDIO = -L../TinyAudioLibrary -ltiny
LPORTAUDIO = -lportaudio
PROGS = hellotable ring-modulator
PORTSF = ../portsf/lib/libportsf.a
TINYAUDIO = ../TinyAudioLibrary/libtiny.a
CC = gcc -Wall -std=c99

all: $(PROGS)

$(PORTSF):
	cd ../portsf/portsf; make; make install

$(TINYAUDIO):
	cd ../TinyAudioLibrary; make; make install

hellotable: hellotable.c $(TINYAUDIO)
	$(CC) -o hellotable hellotable.c $(INCLUDES) $(LIBS) $(LTINYAUDIO) $(LPORTAUDIO)

ring-modulator: ring-modulator.c
	$(CC) -o ring-modulator ring-modulator.c $(LPORTAUDIO)

clean:
	rm -f $(PROGS)
	rm -f *.o

veryclean: clean
	cd ../portsf/portsf; make veryclean
