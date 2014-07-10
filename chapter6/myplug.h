#include <string.h>
#include "public.sdk/source/vst2.x/audioeffectx.h"

enum { PROGS = 64, GAIN = 0, PITCH, FDB, DSIZE, PARAMS };
struct  MyPlugProgram {
	MyPlugProgram()
	{
		gain = 0.5;
		pitch = 0.5;
		fdb = 0.0;
		dsize = 0.045;
		strcpy(name, "Init");
	}

	float gain;
	float pitch;
	float fdb;
	float dsize;
	char name[64];
};

class MyPlug : public AudioEffectX
{
	protected:
		MyPlugProgram *programs;
		float *delay; // circular buffer
		float *env;
		float gain;
		float pitch;
		float rp;
		float fdb;
		float sr;
		int taps;
		int dsize;
		int wp; // write-point

	public:
		MyPlug(audioMasterCallback audioMaster);
		~MyPlug();
		virtual void resume();
		virtual bool getEffectName(char *name);
		virtual bool getVendorString(char *text);
		virtual bool getProductString(char *text);
		virtual VstInt32 getVendorVersion() { return 1024; }
		virtual VstPlugCategory getPlugCategory() { return kPlugCategEffect; }

		virtual void processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames);

		virtual void setParameter(VstInt32 index, float value);
		virtual float getParameter(VstInt32 index);
		virtual void getParameterLabel(VstInt32 index, char *label);
		virtual void getParameterDisplay(VstInt32 index, char *text);
		virtual void getParameterName(VstInt32 index, char *text);
		virtual void setProgram(VstInt32 program);
		virtual void setProgramName(char *name);
		virtual void getProgramName(char *name);
		virtual bool getProgramNameIndexed(VstInt32 category, VstInt32 index, char *text);

		void setGain(float g)
		{
			gain = g;
		}

		void setPitch(float p)
		{
			pitch = p;
		}

		void setFdb(float f)
		{
			fdb = f;
		}

		void setDsize(float d);
};
