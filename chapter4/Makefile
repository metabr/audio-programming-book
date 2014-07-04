INCLUDES = -I../portsf/include -I.
LIBS = -L../portsf/lib -lportsf -lm
PORTSF = ../portsf/lib/libportsf.a
CC = gcc -Wall $(INCLUDES)

waveform: rd.o out.o main.o $(PORTSF)
	$(CC) -o waveform rd.o out.o main.o $(LIBS)

rd.o: rd.c waveform.h
	$(CC) -c -g rd.c

out.o: out.c waveform.h $(PORTSF)
	$(CC) -c -g out.c

main.o: main.c waveform.h $(PORTSF)
	$(CC) -c -g main.c

$(PORTSF):
	cd ../portsf/portsf; make; make install

clean:
	rm -f waveform
	rm -f *.o

veryclean: clean
	cd ../portsf/portsf; make veryclean
