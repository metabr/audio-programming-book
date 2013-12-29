#include <stdlib.h>
#include "sms.h"

#define TRUE 1
#define FALSE 0

int analyze(char* inputFile)
{
    SMS_Data analysisData;
    SMS_Header smsHeader;
    SMS_SndHeader soundHeader;
    SMS_AnalParams analysisParams;
   
    FILE *smsFile;
    float currentFrame[SMS_MAX_WINDOW];
    int doAnalysis = TRUE, numFrames = 0;
    long status = 0, numSamplesRead = 0, frameSize = 0;
    
    /* open input sound */
    if(sms_openSF(inputFile, &soundHeader))
    {
        printf("Error: Could not open the input sound file\n");
        exit(1);
    }	 

    /* initialize libsms */
    sms_init();
    sms_initAnalParams(&analysisParams);
    analysisParams.iFrameRate = 344;
    sms_initAnalysis(&analysisParams, &soundHeader);
    sms_fillHeader(&smsHeader, &analysisParams, "AnalysisParameters");
    sms_writeHeader("analysis_data.sms", &smsHeader, &smsFile);
    sms_allocFrameH(&smsHeader, &analysisData);

    /* analysis loop */
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
            printf("Error processing frame: %s\n", 
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
            /* write this frame of analysis data to disk */
            sms_writeFrame(smsFile, &smsHeader, &analysisData);
            numFrames++;
        }

        /* if the analysis status is 1, analysis was successful 
         * and we have a frame of analysis data 
         */
        else if(status == -1)
        {
            doAnalysis = FALSE;
            smsHeader.nFrames = numFrames;
        }
    }

    /* close input sound file */
    sms_closeSF();
    
    /* close the file containing our analysis data */
    sms_writeFile(smsFile, &smsHeader);

    /* free memory used by libsms */
    sms_freeAnalysis(&analysisParams);
    sms_free();
}

void synthesize(char* outputFile, float timeStretch)
{
    SMS_Header *smsHeader;
    SMS_Data smsFrame; 
    SMS_SynthParams synthParams;
    synthParams.iSynthesisType = SMS_STYPE_ALL;
    synthParams.iDetSynthType = SMS_DET_IFFT;
    synthParams.sizeHop = 128;

    FILE *smsFile;
    float *audioOutput;
    int numSamples = 0, numSamplesWritten = 0, currentFrame = 0;

    /* read the SMS_Header information from the analysis file */
    sms_getHeader("analysis_data.sms", &smsHeader, &smsFile);

    /* initialize libsms */
    sms_init();
    sms_initSynthParams(&synthParams);
    sms_initSynth(smsHeader, &synthParams);
    sms_allocFrameH(smsHeader, &smsFrame); 

    /* initialize libsndfile for writing a soundfile */
    sms_createSF(outputFile, synthParams.iSamplingRate, 0);

    /* allocate memory for one frame of audio output */
    if((audioOutput = (float *)calloc(synthParams.sizeHop, 
       sizeof(float))) == NULL)
    {
        printf("Could not allocate memory for audio output\n");
        exit(1);
    }

    /* calculate the total number of samples that will 
     * be written to the output file 
     */
    numSamples = smsHeader->nFrames * synthParams.sizeHop * 
                 timeStretch;

    /* synthesis loop */
    while(numSamplesWritten < numSamples)
    {	
        /* calculate the current frame number based on 
         * the time stretch factor 
         */
        currentFrame = numSamplesWritten / 
                       (synthParams.sizeHop * timeStretch);

        /* read in a frame of analysis data */
        sms_getFrame(smsFile, smsHeader, currentFrame, &smsFrame);

        /* synthesise audio output */
        sms_synthesize(&smsFrame, audioOutput, &synthParams);

        /* write to output file */
        sms_writeSound(audioOutput, synthParams.sizeHop);

        /* update the number of samples written */
        numSamplesWritten += synthParams.sizeHop;
    }
    
    /* close output sound file */
    sms_writeSF();

    /* free memory used by libsms */
    sms_freeFrame(&smsFrame);
    sms_freeSynth(&synthParams);
    sms_free();
}

int main(int argc, char *argv[])
{
    /* make sure that we have the correct number of 
     * command line arguments 
     */
    if(argc != 4)
    {
        printf("Error: please specify an input file, an output file "
               "and a time stretch factor\n");
        exit(1);
    }

    char *inputFile = argv[1];
    char *outputFile = argv[2];
    float timeStretch = atof(argv[3]);

    analyze(inputFile);
    synthesize(outputFile, timeStretch);

    return 0;
}
