#include "snd_def.h"

SNDFILE *soundout_open(char *name, int chans, float srate)
{
	SF_INFO info;
	info.samplerate = (int)srate;
	info.channels = chans;
	info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

	return sf_open(name, SFM_WRITE, &info);
}

void soundout_close(SNDFILE *psf_out)
{
	sf_close(psf_out);
}

SNDFILE *soundin_open(char *name, int *chans, float *srate)
{
	SF_INFO info;
	SNDFILE *sf = sf_open(name, SFM_READ, &info);
	*srate = (float)info.samplerate;
	*chans = info.channels;
	return sf;
}

void soundin_close(SNDFILE *psf_in)
{
	sf_close(psf_in);
}

int soundout(SNDFILE *psf_out, float *buffer, int vsize)
{
	return (int)sf_writef_float(psf_out, buffer, vsize);
}

int soundin(SNDFILE *psf_in, float *buffer, int vsize)
{
	return (int)sf_readf_float(psf_in, buffer, vsize);
}

long dataframes(SNDFILE *psf)
{
	long size = sf_seek(psf, 0, SEEK_END);
	sf_seek(psf, 0, SEEK_SET);
	return size;
}
