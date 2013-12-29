
#include "Harmbank.hpp"

#if MAC
#pragma export on
#endif

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	AudioEffect* ae(0);
	try{
		ae =  new Harmbank (audioMaster);
	}
	catch(const HarmBankException& x){
		ae = 0; // just in case!
	}	
	return ae;
}
