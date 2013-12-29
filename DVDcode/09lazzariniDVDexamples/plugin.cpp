/* this is our plugin class header file */
#include "plugin.h"

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
    return new Plugin(audioMaster);
}


Plugin::Plugin (audioMasterCallback audioMaster)
  : AudioEffectX (audioMaster, 0, 0){ }

Plugin::~Plugin() { }

void Plugin::processReplacing(float** inputs, 
              float** outputs, VstInt32 vecframes)
{	
  float *in = inputs[0];
  float *out1 = outputs[0], *out2 = outputs[1];
  // processing loop    
  for(int i=0; i < vecframes; i++){
   out1[i] = in[i]; out2[i] = in[i];
  }
} 
