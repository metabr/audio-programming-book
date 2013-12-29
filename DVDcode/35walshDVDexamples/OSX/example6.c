#include <stdio.h>
#include "csound.h"

/*-------------*/
/* example6.c  */
/*-------------*/

/*-------------------------------------------------------------
//performance thread and write file function prototypes
//------------------------------------------------------------*/
uintptr_t csThread(void *clientData);
int CreateInputFile(char* csdFileName);

/*-------------------------------------------------------------
//userData structure declaration
//------------------------------------------------------------*/
typedef struct {
int result;
CSOUND* csound;
int PERF_STATUS;
}userData;

//------------------------------------------------------------
// main function
//------------------------------------------------------------
int main(int argc, char *argv[])
{
int userInput=200;
void* ThreadID;
int result=1;
userData* ud;
/* create string array to pass a input arguments */
int noOfArgs=2;
char** inputArgs = (char **) malloc(sizeof(char*)*(2));
inputArgs[0] = "csound";
inputArgs[1] = "input.csd";

ud = (userData *)malloc(sizeof(userData));
MYFLT* pvalue;

/* create input file */
result = CreateInputFile(inputArgs[1]);
csoundInitialize(&noOfArgs, &inputArgs, 0); 

ud->csound=csoundCreate(NULL);
/* we no longer use argv, instead we pass inputArgs */
ud->result=csoundCompile(ud->csound,noOfArgs,inputArgs);
if(!ud->result)
  {
  ud->PERF_STATUS=1;
  ThreadID = csoundCreateThread(csThread, (void*)ud);
  }
else{
printf("csoundCompiled returned an error");
return 0;
}

printf("\nEnter a pitch in Hz(0 to Exit) and type return\n"); 

while(userInput!=0)
     {
     if(csoundGetChannelPtr(ud->csound, &pvalue, "pitch",
                    CSOUND_INPUT_CHANNEL | CSOUND_CONTROL_CHANNEL)==0);    
     *pvalue = (MYFLT)userInput; 
     scanf("%d", &userInput);                           
     }
ud->PERF_STATUS=0;
csoundDestroy(ud->csound);
free(ud);
free(inputArgs);

return 1;
}

//-------------------------------------------------------------
//definition of our performance thread function
//-------------------------------------------------------------
uintptr_t csThread(void *data)
{
	userData* udata = (userData*)data;
     	if(!udata->result)
        {
             while((csoundPerformKsmps(udata->csound) == 0)
			&&(udata->PERF_STATUS==1));
		csoundDestroy(udata->csound);
        }       
	udata->PERF_STATUS = 0;   
      return 1;
}

//-------------------------------------------------------------
//write a CSD file to disk...
//-------------------------------------------------------------
int CreateInputFile(char* csdFileName){
int result=0;
int i, octave, pitch, amplitude, startTime;
startTime = 0;
const char* fileContents;
/* create a file pointer */
FILE *csdFile;
/* open a file for writing */
csdFile=fopen(csdFileName, "w");
/* make sure the file opened properly */
if(csdFile == 0){
	printf("Could not create Csound file.\n");
	return 0;
	}
else{
/* create a string that contains our Csound code but leave out the complete score, this will be written in the for loop that follows */
const char* fileContents= "<CsoundSynthesizer>\n"
"<CsOptions>\n"
"-odac -g\n"
"</CsOptions>\n"
"<CsInstruments>\n"
"sr = 44100\n"
"ksmps = 64\n"
"nchnls = 2\n"
"\n"
"instr 1\n"
"kscal active  1 \n"
"kamp port   2/kscal, 0.01 \n"
"k1 expon p4, p3, 0.001\n"
"kfreq tab p5, 2\n"
"a1stVoice oscil k1, cpspch((p6+6)+kfreq), 1\n"
"a2ndVoice oscil k1, cpspch((p6+5)+kfreq), 1\n"
"a2ndVoice delay a2ndVoice, 5.5\n"
"acombL comb a1stVoice, 4, 3.5\n" 
"acombR comb a1stVoice, 5, 2.\n"
"outs (acombL+a1stVoice+a2ndVoice)*kamp,  (acombR+a1stVoice+a2ndVoice)*kamp\n"
"endin\n"
"\n"
"</CsInstruments>  \n"
"<CsScore>\n"
"f1 0 1024 10 1\n"
"f2 0 8 2 .00 .02 .04 .05 .07 .09 .11 1\n";
/* write this much of the csd file to disk */
result = fprintf(csdFile,  fileContents);
if(result<0){
	printf("Error writing data to file");
	return 0;
}
else{

/* write 100 notes in the form of score events, of variable length, pitch and amplitude */
for(i=0;i<100;i++){
octave = abs(rand()%4);
pitch = abs(rand()%8);
pitch = pitch+octave;
amplitude = abs(rand()%10000)+10000;
startTime = startTime+rand()%3;
fprintf(csdFile, "i1 %d 10 %d %d %d\n", startTime, amplitude, pitch, octave);
}
}
/* finish the score and append it to the csd file */
fileContents = "</CsScore>\n"
"</CsoundSynthesizer>";

result = fprintf(csdFile,  fileContents);
if(result<0){
	printf("Error writing data to file");
	return 0;
}

}
fclose(csdFile);
}
