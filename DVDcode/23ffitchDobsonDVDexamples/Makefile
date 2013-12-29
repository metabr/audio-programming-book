##Linux
# LIBS = -lfltk -ldl -lm -lX11
# LIBSE = -lsndfile -lportaudio

##Windows

##Mac
LIBS = -lfltk -lm
LIBSE = -lsndfile -lportaudio

##In addition to FLTK needs libsndfile and PortAudio

FilterResponse: FilterResponse.cxx FilterResponse.h
	g++ -o FilterResponse -O3 -g FilterResponse.cxx -I/usr/local/include -L/usr/local/lib $(LIBS) $(LIBSE)
