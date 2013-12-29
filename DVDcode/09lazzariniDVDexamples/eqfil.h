#ifndef __eqfil__
#define __eqfil__

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <string.h>

enum { PROGS = 64, FREQ = 0, QU, GAIN, PARAMS };

class Eqfil : public AudioEffectX
{
  struct Program{
    Program(){gain=0.25f;freq=.1f;q=0.f;strcpy(name, "default");}	
    float gain;
    float freq;
    float q;
    char name[64];
   }  *programs;

  float z1,z2;
  float gain, freq, q;
  double a,d,g;
  bool param_changed;

 public:
  Eqfil (audioMasterCallback audioMaster);
  ~Eqfil ();

  void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
  void setProgram (VstInt32 program);
  void setProgramName (char* name);
  void getProgramName (char* name);
  bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);
  void setParameter (VstInt32 index, float value);
  float getParameter (VstInt32 index);
  void getParameterLabel (VstInt32 index, char* label);
  void getParameterDisplay (VstInt32 index, char* text);
  void getParameterName (VstInt32 index, char* text);
  void setSampleRate(float sr){param_changed=true; AudioEffectX::setSampleRate(sr);}
  void resume ();
  bool getEffectName (char* text) { strcpy(text,"equalizer filter"); }
  bool getVendorString (char* text) { strcpy(text,"acme software"); }
  bool getProductString (char* text)  { strcpy(text,"equalizer filter"); }
  VstInt32 getVendorVersion () { return 1024; }
  VstPlugCategory getPlugCategory () { return kPlugCategEffect; }  
 
};

#endif
