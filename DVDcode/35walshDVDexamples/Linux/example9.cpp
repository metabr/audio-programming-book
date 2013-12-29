#include <stdio.h>
#include <iostream>
#include <istream>
#include "csound.hpp"
#include "csPerfThread.hpp"

/*---------------*/
/* example9.cpp  */
/*---------------*/

using namespace std;

int main(int argc, char *argv[])
{
/*Create an instance of Csound*/
Csound* csound = new Csound;
/*pre-compile instance of csound*/
csound->PreCompile();
/*compile instance of csound*/
csound->Compile("example9.csd");

/* set up CsoundPerfThread object */
CsoundPerformanceThread* perf = new CsoundPerformanceThread(csound); 

/* start csound thread */
perf->Play();
	
cout << "Press any number between 1 and 5 to start a pattern of notes. Press any number above 5 to exit\n";

int loop = 1;
while(loop < 6)
{
cin >> loop;
csound->SetChannel("loop", loop+2);
}

/* stop thread */
perf->Stop();
/*delete instance of csound*/
delete csound;
}
