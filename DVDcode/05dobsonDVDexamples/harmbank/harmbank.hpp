/* Copyright (c) 2009 Richard Dobson

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// © 2003, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __HARMBANK_H
#define __HARMBANK_H

#include "audioeffectx.h"
#include <stdlib.h>
#include "filterbank.h"

#define MAX_FILTERS (24)
#define SEMITONE (1.0594631)
#define InvRandMax (1.0 / RAND_MAX)



enum {HB_TOPFREQ,HB_BOTFREQ,HB_Q,HB_WARP,HB_GAIN };

class HarmBankException {};

// RWD note: the virtual specifiers are unneccessary: once a function is declared virtual in a base class, it
// is automatically virtual in derived classes. Adding the virtual specifer is legal but redundant 
// - like casting a double to a double. VST plugin methods have always been declared virtual in this way
// as documentation. This tradition is respected here! According to some authorities, this is also "good style".
//-------------------------------------------------------------------------------------------------------
class Harmbank : public AudioEffectX
{
public:
	Harmbank (audioMasterCallback audioMaster);
	~Harmbank ();
	// Program
	virtual void setProgramName (char *name);
	virtual void getProgramName (char *name);
	// Parameters
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char *label);
	virtual void getParameterDisplay (VstInt32 index, char *text);
	virtual void getParameterName (VstInt32 index, char *text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion () { return 1000; }
	virtual VstPlugCategory getPlugCategory () { return kPlugCategEffect; }
    // control
    virtual void resume();
    // Processing
    // this is deprecated in VST 2.4!
//	virtual void process (float **inputs, float **outputs, VstInt32 sampleFrames);
	virtual void processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames);
protected:
	RBJ_FILTERBANK filterbank;
	double		filter_freqs[MAX_FILTERS];
	int			nfilters;
	double		nyquist,d_srate,this_output,filter_botfreq,filter_topfreq,filter_Q,filter_gain;
	double		prev_topfreq, prev_warp,prev_botfreq,prev_Q;
	float		fGain,fTopfreq,fBotfreq,fQ,fWarp;
	char		programName[kVstMaxProgNameLen];
};

#endif
