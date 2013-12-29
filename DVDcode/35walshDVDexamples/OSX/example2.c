#include <stdio.h>
#include "csound.h"

/*-------------*/
/* example2.c  */
/*-------------*/

int main(int argc, char *argv[])
{
/* create string array to pass a input arguments */
int noOfArgs = 2;
char** inputArgs = (char **) malloc(sizeof(char*)*(2));
inputArgs[0] = "csound";
inputArgs[1] = "example2.csd";

csoundInitialize(&noOfArgs, &inputArgs, 0); 
CSOUND*csound=csoundCreate(0);

int result=csoundCompile(csound,noOfArgs,inputArgs);

/* declare a pointer to a variable of type MYFLT */
MYFLT *pvalue;

if(!result)
{
while(csoundPerformKsmps(csound)==0){
	/*call csoundGetChannelPtr to send the value of
	'pvalue' to 'csound' on a channel named 'pitch' */
	if(csoundGetChannelPtr(csound, &pvalue, "pitch",                     
	CSOUND_INPUT_CHANNEL | CSOUND_CONTROL_CHANNEL) == 0)
   	*pvalue = 200;                                  
}

}
//free inputargs
free(inputArgs);
csoundDestroy(csound);
return 1;
}