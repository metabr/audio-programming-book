#include "sms.h"

#define TRUE 1
#define FALSE 0

int analyze(char* inputFile, char* dataFile)
{
    SMS_Data analysisData;
    SMS_Header smsHeader;
    SMS_SndHeader soundHeader;
    SMS_AnalParams analysisParams;
   
    FILE* smsFile;
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
    analysisParams.specEnvParams.iType = SMS_ENV_FBINS;
    analysisParams.specEnvParams.iOrder = 80;
    sms_initAnalysis(&analysisParams, &soundHeader);
    sms_fillHeader(&smsHeader, &analysisParams, "AnalysisParameters");
    sms_allocFrameH(&smsHeader, &analysisData);
    sms_writeHeader(dataFile, &smsHeader, &smsFile);

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

    /* close the input sound file */
    sms_closeSF();
    
    /* close the file containing our analysis data */
    sms_writeFile(smsFile, &smsHeader);

    /* free memory used by libsms */
    sms_freeAnalysis(&analysisParams);
    sms_free();
}

void synthesize(char* outputFile)
{
    SMS_Header *sourceSmsHeader, *targetSmsHeader;
    SMS_Data sourceSmsFrame, targetSmsFrame; 
    SMS_SynthParams synthParams;
    synthParams.sizeHop = 128;
    SMS_ModifyParams modifyParams;

    FILE *sourceSmsFile, *targetSmsFile;
    float *audioOutput;
    int numFrames = 0, currentFrame = 0, envPosition = 0;

    /* read the SMS_Header information from the analysis files */
    sms_getHeader("source_file.sms", &sourceSmsHeader, &sourceSmsFile);
    sms_getHeader("target_file.sms", &targetSmsHeader, &targetSmsFile);

    /* initialize libsms */
    sms_init();
    sms_allocFrameH(sourceSmsHeader, &sourceSmsFrame); 
    sms_allocFrameH(targetSmsHeader, &targetSmsFrame); 
    sms_initSynthParams(&synthParams);
    sms_initSynth(targetSmsHeader, &synthParams);
    sms_initModifyParams(&modifyParams);
    sms_initModify(targetSmsHeader, &modifyParams);

    /* initialize libsndfile for writing a soundfile */
    sms_createSF(outputFile, synthParams.iSamplingRate, 0);

    /* allocate memory for one frame of audio output */
    if((audioOutput = (float *)calloc(synthParams.sizeHop, 
       sizeof(float))) == NULL)
    {
        printf("Could not allocate memory for audio output\n");
        exit(1);
    }

    /* set the number of output frames to be the minimum of the 
     * number of frames in each file
     */
    if(sourceSmsHeader->nFrames < targetSmsHeader->nFrames)
    {
        numFrames = sourceSmsHeader->nFrames;
    }
    else
    {
        numFrames = targetSmsHeader->nFrames;
    }

    /* we are going to modify the harmonic component using a spectral
     * envelope, so set this to true
     */
    modifyParams.doSinEnv = TRUE;
    
    /* this value lets us interpolate between two spectral envelopes. 
     * If it is 0, the original envelope is used, if it is 1 the new 
     * envelope is used, and any value in between will result in the
     * appropriate linear interpolation between the two.
     */
    modifyParams.sinEnvInterp = 1; 

    /* synthesis loop */
    while(currentFrame < numFrames)
    {	
        /* read in both frames of analysis data */
        sms_getFrame(sourceSmsFile, sourceSmsHeader, currentFrame, 
                     &sourceSmsFrame);
        sms_getFrame(targetSmsFile, targetSmsHeader, currentFrame, 
                     &targetSmsFrame);

        /* copy the source envelope into the modification 
         * parameters structure 
         */
        for(envPosition = 0; envPosition < modifyParams.sizeSinEnv; 
            envPosition++)
        {
            modifyParams.sinEnv[envPosition] = 
                sourceSmsFrame.pSpecEnv[envPosition];
        }

        /* call sms_modify to apply the new envelope */
        sms_modify(&targetSmsFrame, &modifyParams);

        /* synthesise audio output */
        sms_synthesize(&targetSmsFrame, audioOutput, &synthParams);

        /* write to output file */
        sms_writeSound(audioOutput, synthParams.sizeHop);
        currentFrame++;
    }
    
    /* close output sound file */
    sms_writeSF();

    /* free memory used by libsms */
    sms_freeFrame(&sourceSmsFrame);
    sms_freeFrame(&targetSmsFrame);
    sms_freeSynth(&synthParams);
    sms_freeModify(&modifyParams);
    sms_free();
}

int main(int argc, char *argv[])
{
    /* make sure that we have the correct number of 
     * command line arguments 
     */
    if(argc != 4)
    {
        printf("Error: please specify a source file, a target file "
               "and an output file\n");
        exit(1);
    }

    char *sourceFile = argv[1];
    char *targetFile = argv[2];
    char *outputFile = argv[3];

    analyze(sourceFile, "source_file.sms");
    analyze(targetFile, "target_file.sms");
    synthesize(outputFile);

    return 0;
}
