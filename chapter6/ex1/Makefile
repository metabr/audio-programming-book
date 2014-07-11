env_test: soundio.o tables.o env.o oscil.o env_test.cpp
	g++ env_test.cpp soundio.o tables.o env.o oscil.o -o env_test -lsndfile

soundio.o: soundio.cpp
	g++ -c soundio.cpp

oscil.o: oscil.cpp
	g++ -c oscil.cpp

tables.o: tables.cpp
	g++ -c tables.cpp

env.o: env.cpp
	g++ -c env.cpp

filter.o: filter.cpp
	g++ -c filter.cpp

butterworth.o: butterworth.cpp
	g++ -c butterworth.cpp

clean:
	rm -f *.o
	rm -f env_test
