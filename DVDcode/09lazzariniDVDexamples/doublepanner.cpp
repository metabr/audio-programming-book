/* this is our plugin class header file */
#include "doublepanner.h"
#include <cstdio>

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new Plugin(audioMaster);
}


Plugin::Plugin (audioMasterCallback audioMaster)
  : AudioEffectX (audioMaster, 64, 2){ 
pan1 = 0.5f; pan2 = 0.5f;
 setNumInputs(2);
}

Plugin::~Plugin() { }

void Plugin::processReplacing(float** inputs, 
              float** outputs, VstInt32 vecframes)
{	
  float *in1 = inputs[0], *in2 = inputs[1];
  float *out1 = outputs[0], *out2 = outputs[1];
  // processing loop    
  for(int i=0; i < vecframes; i++){
   out1[i] = (1.f-pan1)*in1[i] + (1.f-pan2)*in2[i]; 
   out2[i] = (pan1)*in1[i] + (pan2)*in2[i];
  }

} 

void Plugin::setParameter (VstInt32 index, float value){

    switch(index){
      case 0:
      progs[curProgram].pan1 = pan1 = value; return;
      case 1:
      progs[curProgram].pan2 = pan2 = value; return;
	default:
	return;
    }
}


float Plugin::getParameter (VstInt32 index) { 

switch(index){
        case 0:
	  return pan1;
        case 1:
        return pan2;
	  default:
	  return 0;
      }
}

void Plugin::setProgramName (char *name)
{
  strcpy(progs[curProgram].name, name);
}

void Plugin::getProgramName(char *name)
{
  if (!strcmp (progs[curProgram].name, "default"))
    sprintf(name, "%s %d", progs[curProgram].name, 
                          curProgram + 1);
  else
    strcpy(name, progs[curProgram].name);
}

bool Plugin::getProgramNameIndexed(VstInt32 category, 
             VstInt32 index, char* text)
{
  if (index < 64)
    {
      strcpy(text, progs[index].name);
      return true;
    }
  return false;
}

void Plugin::getParameterName(VstInt32 index, char *label)
{
  switch (index)
    {
    case 0: 
      strcpy (label, "pan-left in");  
      break;
    case 1: 
      strcpy (label, "pan-right in");
      break;
    }
}

void Plugin::getParameterLabel (VstInt32 index, char *label)
{
      strcpy (label, "position");  
     
}

void Plugin::getParameterDisplay (VstInt32 index, char *text)
{
    switch (index)
    {
    case 0: 
      float2string (pan1, text, kVstMaxParamStrLen);
      break;
    case 1: 
      float2string (pan2, text, kVstMaxParamStrLen);
      break;
    }
}

void Plugin::setProgram(VstInt32 program){
setParameter(0,progs[program].pan1);
setParameter(1,progs[program].pan2);
curProgram = program;

}
