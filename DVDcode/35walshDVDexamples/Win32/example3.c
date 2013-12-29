#include <stdio.h>
#include "csound.h"

/*-------------*/
/* example3.c  */
/*-------------*/

/* callback prototype */
void inputValueCallback(CSOUND *csound,  const char *channelName,
MYFLT *value);

int main(int argc, char *argv[])
{
/* create string array to pass a input arguments */
int noOfArgs = 2;
char** inputArgs = (char **) malloc(sizeof(char*)*(2));
inputArgs[0] = "csound";
inputArgs[1] = "example3.csd";

csoundInitialize(&noOfArgs, &inputArgs, 0); 
CSOUND*csound=csoundCreate(0);

int result=csoundCompile(csound,noOfArgs,inputArgs);
csoundSetInputValueCallback(csound, inputValueCallback);

if(!result){
while(csoundPerformKsmps(csound)==0);
}

csoundDestroy(csound);
free(inputArgs);
return result;
}

/* callback function */
void inputValueCallback(CSOUND *csound,  const char *channelName,
MYFLT *value){
if(!strcmp(channelName, "amp"))
*value = 20000;
}