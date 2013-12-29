GCC = gcc

all: simple_compressor simple_compressor_log lookahead_compressor rms_compressor knee_compressor

simple_compressor: simple_compressor.c main.c
	$(GCC) -o simple_compressor main.c -DSIMPLE_COMPRESSOR -I/usr/local/include -L/usr/local/lib -lportaudio -lsndfile

simple_compressor_log: simple_compressor_log.c main.c
	$(GCC) -o simple_compressor_log main.c -DSIMPLE_COMPRESSOR_LOG -I/usr/local/include -L/usr/local/lib -lportaudio -lsndfile

lookahead_compressor: lookahead_compressor.c main.c
	$(GCC) -o lookahead_compressor main.c -DLOOKAHEAD_COMPRESSOR -I/usr/local/include -L/usr/local/lib -lportaudio -lsndfile

rms_compressor: rms_compressor.c main.c
	$(GCC) -o rms_compressor main.c -DRMS_COMPRESSOR -I/usr/local/include -L/usr/local/lib -lportaudio -lsndfile

knee_compressor: knee_compressor.c main.c
	$(GCC) -o knee_compressor main.c -DKNEE_COMPRESSOR -I/usr/local/include -L/usr/local/lib -lportaudio -lsndfile