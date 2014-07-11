#ifndef _SND_DEF_H_
#define _SND_DEF_H_

#include <math.h>
#include <memory.h>
#include <sndfile.h>

const int default_vsize = 64;
const float default_srate = 44100.0;
const float default_crate = default_srate / default_vsize;
const int default_chans = 1;
const int default_length = 1024;

/* Butterworth filter modes */
enum { BP, BR, LP, HP };

/* Open RIFF-Wave format output soundfile.
 *
 * name: soundfile name (*.wav)
 * chans: number of output channels
 * srate: sample rate
 *
 * returns: soundfile handle, 0 if failed
 */
SNDFILE *soundout_open(char *name, int chans=default_chans, float srate=default_srate);

/* Close output soundfile */
void soundout_close(SNDFILE *psf_out);

/* Open input soundfile (any format supported by libsndfile
 *
 * name: soundfile name
 * chans: will hold number of channels
 * srate: will hold sample rate
 *
 * returns: soundfile handle, 0 if failed
 */
SNDFILE *soundin_open(char *name, int *chans, float *srate);

/* Close input soundfile */
void soundin_close(SNDFILE *psf_in);

/* Output to soundfile
 *
 * psf_out: file handle
 * buffer: signal buffer (interleaved for multi-channel)
 * vsize: vector size in frames
 *
 * returns: number of frames output to file
 */
int soundout(SNDFILE *psf_out, float *buffer, int vsize=default_vsize);

/* Input from soundfile
 *
 * psf_in: file handle
 * buffer: signal buffer (interleaved for multi-channel)
 * vsize: vector size in frames
 *
 * returns: number of frames read from file
 */
int soundin(SNDFILE *psf_in, float *buffer, int vsize=default_vsize);

/* Get number of frames in file
 *
 * psf: file handle
 *
 * returns: number of frames
 */
long dataframs(SNDFILE *psf);

#endif
