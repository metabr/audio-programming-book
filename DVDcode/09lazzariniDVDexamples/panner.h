/* this is the base class header file */
#include "public.sdk/source/vst2.x/audioeffectx.h"

class Plugin : public AudioEffectX
{
  float pan;
 public:
 /* initialization and termination */
  Plugin (audioMasterCallback audioMaster);
  ~Plugin ();
  
 /* processing */
  void processReplacing (float** inputs, float** outputs, 
VstInt32 sampleFrames);
 void setParameter (VstInt32 index, float value);
  float getParameter (VstInt32 index);


};
