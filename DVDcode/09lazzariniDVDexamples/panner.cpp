/* this is our plugin class header file */
#include "panner.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new Plugin(audioMaster);
}


Plugin::Plugin (audioMasterCallback audioMaster)
  : AudioEffectX (audioMaster, 0, 1){ pan = 0.5f;}

Plugin::~Plugin() { }

void Plugin::processReplacing(float** inputs, 
              float** outputs, VstInt32 vecframes)
{	
  float *in = inputs[0];
  float *out1 = outputs[0], *out2 = outputs[1];
  // processing loop    
  for(int i=0; i < vecframes; i++){
   out1[i] = (1.f-pan)*in[i]; out2[i] = (pan)*in[i];
  }
} 
void Plugin::setParameter (VstInt32 index, float value){
	pan = value;
}

float Plugin::getParameter (VstInt32 index) { return pan; }
