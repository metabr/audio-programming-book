#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <sndfile.h>
#include <portaudio.h>

/* External algorthm files define the mydata struct and
   two functions, one for initialization of data:

int data_init(mydata *p)

   and another which processes audio blocks:

void audio_process()

   These external files are selected according to the macros
   defined in the Makefile.
*/

#ifdef SIMPLE_COMPRESSOR
#include "simple_compressor.c"
#endif

#ifdef SIMPLE_COMPRESSOR_LOG
#include "simple_compressor_log.c"
#endif

#ifdef LOOKAHEAD_COMPRESSOR
#include "lookahead_compressor.c"
#endif

#ifdef RMS_COMPRESSOR
#include "rms_compressor.c"
#endif

#ifdef KNEE_COMPRESSOR
#include "knee_compressor.c"
#endif

#define FILE_BUF_SIZE 1024

int audio_callback(const void *input, void *output,
                   unsigned long frameCount,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData);

int main(int argc, char** argv)
{
  if (argc > 1) {
    SNDFILE *fp1,*fpo;                           /* soundfile handle */
    SF_INFO   sfinfo1, sfinfo2;                  /* soundfile format data */
    int cnt;                                     /* sample block count */
    int n;                                       /* sample index */
    float inf[FILE_BUF_SIZE], outf[FILE_BUF_SIZE]; /* float signal blocks */
    char outfile[64];                              /* output filename */

    fp1 = sf_open(argv[1], SFM_READ, &sfinfo1);    /* open soundfile 1 */
    if(fp1 == NULL){
      printf("Error. Could not open file: %s\n", argv[1]);
      return 1;
    }
    if (argc < 3) {
      printf("Error. Please specify output filename\n");
      return 1;
    }
    fpo = sf_open(argv[2], SFM_WRITE, &sfinfo1);  /* open output soundfile */
    if(fpo == NULL){
      printf("Error. Could not open file: %s\n", argv[2]);
      return 1;
    }
    sf_command(fpo, SFC_SET_CLIPPING, NULL, SF_TRUE); /*clip instead of wrap*/
    mydata *data = (mydata *) calloc(sizeof(mydata),1);
    data_init(data);
    do {                      /* file-writing loop */
      cnt = sf_read_float(fp1, inf, FILE_BUF_SIZE);
      audio_process(inf, outf, cnt, data);
      sf_write_float(fpo, outf, cnt);   /* write the cnt-sample data block */
    }
    while(cnt);
    sf_close(fpo);                      /* close the files */
    sf_close(fp1);
    free(data);
  }
  else {
    PaError err;
    PaDeviceIndex devin,devout, ndev;
    const PaDeviceInfo *info;
    PaStreamParameters inparam, outparam;
    PaStream *handle;
    int i;
    mydata *data = (mydata *) calloc(sizeof(mydata),1);
    data_init(data);

    err = Pa_Initialize();
    if( err == paNoError){
      ndev = Pa_GetDeviceCount();
      for(i=0; i<ndev; i++){
        info = Pa_GetDeviceInfo((PaDeviceIndex) i);
        if(info->maxOutputChannels > 0) printf("output device: ");
        if (info->maxInputChannels > 0) printf("input device: ");
        printf("%d: %s\n", i, info->name);
      }

      printf("choose device for input: ");
      scanf("%d", &devin);
      printf("choose device for output: ");
      scanf("%d", &devout);

      memset(&inparam, 0, sizeof(PaStreamParameters));
      inparam.device = devin;
      inparam.channelCount = 1;
      inparam.sampleFormat = paFloat32;
      memset(&outparam, 0, sizeof(PaStreamParameters));
      outparam.device = (PaDeviceIndex) devout;
      outparam.channelCount = 1;
      outparam.sampleFormat = paFloat32;

      err = Pa_OpenStream(&handle,&inparam,&outparam,SR,BUF,paNoFlag,
                          audio_callback, data);

      if(err == paNoError){
        err = Pa_StartStream(handle);
        if(err == paNoError){
          while(Pa_GetStreamTime(handle) < END);
          Pa_StopStream(handle);
        } else  printf("%s \n", Pa_GetErrorText(err));
        Pa_CloseStream(handle);
      } else  printf("%s \n", Pa_GetErrorText(err));
      Pa_Terminate();
    }  else  printf("%s \n", Pa_GetErrorText(err));

    free(data);
  }
  return 0;
}

int audio_callback(const void *input, void *output,
                   unsigned long frameCount,
                   const PaStreamCallbackTimeInfo *timeInfo,
                   PaStreamCallbackFlags statusFlags,
                   void *userData)
{
  audio_process(input, output, frameCount, userData);
  return paContinue;
}
