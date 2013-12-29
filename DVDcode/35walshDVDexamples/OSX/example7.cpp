#include <stdio.h>
#include "csound.hpp"

/*---------------*/
/* example7.cpp  */
/*---------------*/

int main(int argc, char *argv[])
{
/*Create an instance of Csound*/
Csound* csound = new Csound;
/*pre-compile instance of csound*/
csound->PreCompile();

/*compile instance of csound.
Instead of passing the file
from the command line we can 
pass it directly to the Compile()
function */
csound->Compile("example1.csd");

csound->Perform();	
/*delete instance of csound*/
delete csound;
}
