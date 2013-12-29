#include <stdio.h>
#include "csound.h"

/*-------------*/
/* example4.c  */
/*-------------*/

/*-------------------------------------------------------------
//performance thread function prototype
//------------------------------------------------------------*/
uintptr_t csThread(void *clientData);

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
userData* ud;
ud = (userData *)malloc(sizeof(userData));

MYFLT* pvalue;
/* create string array to pass a input arguments */
int noOfArgs = 2;
char** inputArgs = (char **) malloc(sizeof(char*)*(2));
inputArgs[0] = "csound";
inputArgs[1] = "example4.csd";

csoundInitialize(&noOfArgs, &inputArgs, 0); 
ud->csound=csoundCreate(NULL);
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
