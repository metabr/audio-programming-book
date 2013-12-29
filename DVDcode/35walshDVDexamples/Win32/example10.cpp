#include "CppSound.hpp"
#include "csPerfThread.hpp"
#include <stdio.h>
#include <string>

using namespace std;

int main()
{
/* create string that holds an entire csd file */
std::string csdText = "<CsoundSynthesizer> \n\
<CsOptions> \n\
csound -odac  temp.orc temp.sco \n\
</CsOptions>  \n\
<CsInstruments>  \n\
sr = 44100  \n\
kr = 4410  \n\
ksmps = 10  \n\
nchnls = 1  \n\
instr 1  \n\
k1 chnget \"freq\" \n\
a1 oscil 10000, k1+100, 1  \n\
out a1  \n\
endin   \n\
</CsInstruments>   \n\
<CsScore>  \n\
f1 0 1024 10 1  \n\
i1 0 100    \n\
</CsScore> \n\
</CsoundSynthesizer>";

int hold;
/*create CppCsound class */
CppSound csound;
/* set the csd text */
csound.setCSD(csdText);
/* export csd text for performance */
csound.exportForPerformance(); 
/* compile csd text */
csound.compile();

/*create performance thread */
CsoundPerformanceThread thread(csound.getCsound());
cout << "Press 1 to play, 2 to pause and Ctrl+C to quit\n"; 
while(1){
cin >> hold;
if(hold==1){
	thread.Play();
	hold = 0;
	}
else if(hold==2){
	thread.Pause();
	hold=0;
	}
}
}
