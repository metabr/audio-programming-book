#include <stdio.h>
#include "csound.hpp"
#include "csPerfThread.hpp"
#include <iostream>

/*---------------*/
/* example8.cpp  */
/*---------------*/

int main(int argc, char *argv[])
{
int i;
/*Create an instance of Csound*/
Csound* csound = new Csound;
/*pre-compile instance of csound*/
csound->PreCompile();
/*compile instance of csound*/
csound->Compile("example8.csd");

/* set up CsoundPerfThread object */
CsoundPerformanceThread* perf = new CsoundPerformanceThread(csound); 

/* start csound thread */
perf->Play();

/* to stop the thread hit a key and then press return */
std::cin >> i;

/* stop thread */
perf->Stop();
/*delete instance of csound*/
delete csound;
delete perf;
return 1;
}
