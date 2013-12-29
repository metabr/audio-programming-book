#ifndef __synth__
#define __synth__

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <string.h>

enum { PROGS = 64, GAIN = 0, ATT, DEC, NDX, WAV, PARAMS };

class Synth : public AudioEffectX
{

  struct Program {
    float gain, att, dec, ndx, wav; char name[64];
    Program() { ndx=gain=1.f;att=0.01f;dec=0.3f;wav=0.f;strcpy(name,"default");}
  } *programs;

  float gain, amp, att, dec, ndx, wav;
  double ph, incr, amp_incr, amp_decr;
  int note, velocity, delta;
  unsigned long cnt, attd, decd;
  bool playing;
  bool envtrig;
  double twopi;

 public:
  Synth (audioMasterCallback audioMaster);
  ~Synth ();

  void processReplacing (float** inputs, float** outputs, VstInt32 vecframes);
  VstInt32 processEvents (VstEvents* events);

  void setProgram (VstInt32 program);
  void setProgramName (char* name);
  void getProgramName (char* name);
  bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

  void setParameter (VstInt32 index, float value);
  float getParameter (VstInt32 index);
  void getParameterLabel (VstInt32 index, char* label);
  void getParameterDisplay (VstInt32 index, char* text);
  void getParameterName (VstInt32 index, char* text);
  bool getEffectName (char* text) { strcpy(text,"sine synth"); }
  bool getVendorString (char* text) { strcpy(text,"acme software"); }
  bool getProductString (char* text)  { strcpy(text,"monosynth"); }
  VstInt32 getVendorVersion () { return 1024; }	
  VstInt32 canDo(char *txt);
	
  inline double note2freq(int note);
  inline float  vel2amp(int vel);

};

#endif
