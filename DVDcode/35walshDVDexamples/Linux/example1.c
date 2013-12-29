#include <stdio.h>
#include "csound.h"

/*-------------*/
/* example1.c  */
/*-------------*/

int main(int argc, char* argv[])
{
/* check that enough arguments have been passed */
if(argc<2){
	printf("Not enough arguments passed to Csound");
	return 0;
	}
/*Initialise the library*/
csoundInitialize(&argc, &argv, 0); 
/*Create an instance of Csound*/
CSOUND *csound=csoundCreate(0);

/*Compile Csound, i.e., run Csound with 
the parameters passed via the command line.
Return false is successful*/
int result=csoundCompile(csound,argc,argv);

/*check to see that Csound compiled Ok, then
start the performance loop. csoundPerformKsmps()
returns false if successful*/
if(!result)
{
while(csoundPerformKsmps(csound)==0);
}

/*Finally destroy Csound*/
csoundDestroy(csound);

return result;
}