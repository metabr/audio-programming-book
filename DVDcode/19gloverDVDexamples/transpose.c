#include <stdlib.h>
#include "sms.h"

#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{
    /* make sure that we have the correct number of 
     * command line arguments 
     */
    if(argc != 4)
    {
    	printf("Error: please specify an input file, an output file "
    	       "and a transposition amount\n");
    	exit(1);
    }

    SMS_Data analysisData;
    SMS_Header smsHeader;
    SMS_SndHeader soundHeader;
    SMS_AnalParams analysisParams;
    SMS_SynthParams synthParams;
    SMS_ModifyParams modifyParams;

    char *inputFile = argv[1];
    char *outputFile = argv[2];
    float transpose = atof(argv[3]);
    float currentFrame[SMS_MAX_WINDOW];
    int doAnalysis = TRUE;
    long status = 0, numSamplesRead = 0, frameSize = 0;
    float *audioOutput;
    
    /* open input sound */
    if(sms_openSF(inputFile, &soundHeader))
    {
        printf("Error: Could not open the input sound file\n");
        exit(1);
    }	    
    
    /* initialize libsms */
    sms_init();
    sms_initAnalParams(&analysisParams);
    
    /* at 44.1 Khz this frame rate results in a hop size of 128 */ 
    analysisParams.iFrameRate = 344;
    sms_initAnalysis(&analysisParams, &soundHeader);
    sms_fillHeader(&smsHeader, &analysisParams, "AnalysisParameters");
    sms_allocFrameH(&smsHeader, &analysisData);
    sms_initSynthParams(&synthParams);
    synthParams.sizeHop = 128;
    sms_initSynth(&smsHeader, &synthParams);
    sms_initModifyParams(&modifyParams);
    sms_initModify(&smsHeader, &modifyParams);

    /* initialize libsndfile for writing a soundfile */
    sms_createSF(outputFile, synthParams.iSamplingRate, 0);

    /* allocate memory for one frame of audio output */
    if((audioOutput = (float *)calloc(synthParams.sizeHop, 
       sizeof(float))) == NULL)
    {
        printf("Error: Could not allocate memory for audio output\n");
        exit(1);
    }
 
    /* set modification parameters to transpose the sound */
    modifyParams.doTranspose = TRUE;
    modifyParams.transpose = transpose;

    /* analysis/synthesis loop */
    while(doAnalysis)
    {
        /* update the number of samples read so far */
        numSamplesRead += frameSize;

        /* get the size of the next frame */
        if((numSamplesRead + analysisParams.sizeNextRead) < 
           soundHeader.nSamples)
        {   
            frameSize = analysisParams.sizeNextRead;
        }
        else
        {   
            frameSize = soundHeader.nSamples - numSamplesRead;
        }

        /* read the next frame */
        if(sms_getSound(&soundHeader, frameSize, currentFrame, 
           numSamplesRead))
        {
            printf("Error: could not read audio frame: %s\n", 
                   sms_errorString());
            break;
        }

        /* analyse the frame */
        status = sms_analyze(frameSize, currentFrame, &analysisData, 
                             &analysisParams);

        /* if the analysis status is 1, analysis was successful 
         * and we have a frame of analysis data 
         */
        if(status == 1)
        {
            /* transpose the sound */
            sms_modify(&analysisData, &modifyParams);
    
            /* synthesise audio output */
            sms_synthesize(&analysisData, audioOutput, &synthParams);
    
            /* write output file */
            sms_writeSound(audioOutput, synthParams.sizeHop);
        }

        /* if status is -1, there is no more data to process, 
         * analysis is finished 
         */
        else if(status == -1)
        {
            doAnalysis = FALSE;
        }
    }

    /* close input sound file */
    sms_closeSF();
    
    /* close output sound file */
    sms_writeSF();

    /* free memory used by libsms */
    sms_freeFrame(&analysisData);
    sms_freeAnalysis(&analysisParams);
    sms_freeSynth(&synthParams);
    sms_freeModify(&modifyParams);
    sms_free();
	
    return 0;
}
