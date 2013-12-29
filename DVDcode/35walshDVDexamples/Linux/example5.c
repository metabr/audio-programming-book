#include <stdio.h>
#include "csound.h"

/*-------------*/
/* example5.c  */
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
  MYFLT userInput[5]={20,0,0,0,0};

void* ThreadID;
userData* ud;
int i;
ud = (userData *)malloc(sizeof(userData));

/* number of pfields expected by instrument 1 */
int numpFields = 5;

MYFLT* pvalue;
/* create string array to pass a input arguments */
int noOfArgs = 2;
char** inputArgs = (char **) malloc(sizeof(char*)*(2));
inputArgs[0] = "csound";
inputArgs[1] = "example5.csd";

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


printf("\n Enter a score event with a start time, a duration, an amplitude value(0-32000) and a pitch value in hz\nExample:\n0 5 10000 440\n  Hit Ctrl+C to exit\n"); 
while(userInput[0]>0)
     {
     for(i=1;i<numpFields;i++){
	    /* retreive user data */  
	    scanf("%f", &userInput[i]);
	    }
	 /* send score event to Csound. Send score events to instrument 20! */
     csoundScoreEvent(ud->csound, 'i', userInput, numpFields);   
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
