/* this is the base class header file */
#include "public.sdk/source/vst2.x/audioeffectx.h"
#include <cstring>


struct Program {
	float pan1, pan2;
   char name[32];
   Program(){ pan1 = 0.5f; pan2 =0.5f; strcpy(name,"default");}
};


class Plugin : public AudioEffectX
{
  Program progs[64];
  float pan1,pan2;
 public:
 /* initialization and termination */
  Plugin (audioMasterCallback audioMaster);
  ~Plugin ();
  
 /* processing */
  void processReplacing (float** inputs, float** outputs, 
VstInt32 sampleFrames);
 void setParameter (VstInt32 index, float value);
  float getParameter (VstInt32 index);
  void  setProgram(VstInt32 program);
   void getProgramName (char* name);
   void setProgramName (char *name);
 bool getProgramNameIndexed (VstInt32 category, 
                    VstInt32 index, char* text);
 void getParameterLabel (VstInt32 index, char* label);
 void getParameterDisplay (VstInt32 index, char* text);
 void getParameterName (VstInt32 index, char* text);
 bool getEffectName (char* name) {
         strcpy (name, "doublepan"); return true; }
 	bool getVendorString (char* text) {
         strcpy (text, "acme software"); return true; }
 	bool getProductString (char* text) {
         strcpy (text, "doublepan"); return true; }
 	VstInt32 getVendorVersion () { return 1024; }
 	VstPlugCategory getPlugCategory () { return kPlugCategEffect; }



};
