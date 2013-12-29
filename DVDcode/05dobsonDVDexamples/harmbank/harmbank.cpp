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
// Example Harmbank (VST 1.0)
// Stereo plugin which applies a Gain [-oo, 0dB]
// © 2003, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __HARMBANK_H
#include "Harmbank.hpp"
#include "filterbank.h"
#endif

#define HB_MAXFREQ (8000.0)

//-------------------------------------------------------------------------------------------------------
Harmbank::Harmbank (audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, 1, 5)	// 1 program, 5 parameters 
{
    /* hardwire some default parameter values */
	fGain = 1.;				
	prev_topfreq = filter_topfreq = fTopfreq = 2000.0;
	prev_botfreq = filter_botfreq = fBotfreq = 100.0;
	prev_Q = filter_Q = fQ		 = 200.0;
	prev_warp = fWarp    = 1.0;
	d_srate = sampleRate; // VST default srate
    filterbank.nfilters = 0;
    // this is the only opportunity to report an error to the host
    // if one of our allocs fails
	if(rbj_createbank(&filterbank,MAX_FILTERS)){
		throw HarmBankException();
	}
    nfilters = 0;
	setNumInputs (1);		// glorious mono
	setNumOutputs (1);		
	setUniqueID ('HmBk');	// identify
    // 2.3: tells host OK to feed both inputs with the same signal
    // deprecated in 2.4
	// canMono ();	 
    // Optional in VST 2.3; mandated in 2.4
    // supports both accumulating and replacing output
	canProcessReplacing ();	
    // set default program name: we have no programs here!
	vst_strncpy (programName, "Default",kVstMaxProgNameLen);	
}

//-------------------------------------------------------------------------------------------------------
Harmbank::~Harmbank ()
{
    // clear internal arrrays
        rbj_filterbank_free(&filterbank);
}

//-------------------------------------------------------------------------------------------------------
void Harmbank::setProgramName (char *name)
{
	vst_strncpy (programName, name,kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Harmbank::getProgramName (char *name)
{
	vst_strncpy (name, programName,kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Harmbank::setParameter (VstInt32 index, float value)
{
	float val;
    
	switch(index){
	case HB_BOTFREQ:
		val = value * HB_MAXFREQ;
		fBotfreq = val > 50.0f ? val : 50.0f;
		break;
	case HB_TOPFREQ:
		val = value * HB_MAXFREQ;
		fTopfreq = val > 50.0f ? val : 50.0f;
		break;
	case HB_Q:
		val = value * 200.0f;
		fQ = val > 1.0f ? val : 1.0f;
		break;
	case HB_WARP:
		val = ((value * 2.0f) - 1.0f) * 0.1f;
		val += 1.0f;
		if(val <= 0.9f) 
			val = 0.9f;
		fWarp = val;
		break;
	case HB_GAIN:
		fGain = value * 10.0f;
		break;
	default:
		break;
	}
	
}

//-----------------------------------------------------------------------------------------
float Harmbank::getParameter (VstInt32 index)
{
	float val;
	switch(index){
	case HB_BOTFREQ:
		val = fBotfreq / HB_MAXFREQ;
		break;
	case HB_TOPFREQ:
		val = fTopfreq / HB_MAXFREQ;
		break;
	case HB_Q:
		val = fQ / 200.0f;
		break;
	case HB_WARP:
		val = (((fWarp - 1.0f) * 10.0f) + 1.0f) * 0.5f;	
		break;
	case HB_GAIN:
		val = fGain * 0.1f;
		break;
	default:
		break;
	}
	return val;
}

//-----------------------------------------------------------------------------------------
void Harmbank::getParameterName (VstInt32 index, char *label)
{
	switch(index){
	case HB_BOTFREQ:
		vst_strncpy (label, "LimFreq",kVstMaxParamStrLen);

		break;
	case HB_TOPFREQ:
		vst_strncpy (label, "RootFrq",kVstMaxParamStrLen);

		break;
	case HB_Q:
		vst_strncpy (label, "Reson",kVstMaxParamStrLen);
		break;
	case HB_WARP:
		vst_strncpy (label, "FrqWarp",kVstMaxParamStrLen);

		break;
	case HB_GAIN:
		vst_strncpy (label, "Gain",kVstMaxParamStrLen);
		break;
	default:
		break;
	}
	
}

//-----------------------------------------------------------------------------------------
void Harmbank::getParameterDisplay (VstInt32 index, char *text)
{
	float val(0.0);

	switch(index){
	case HB_BOTFREQ:
		val = fBotfreq;        
		break;
	case HB_TOPFREQ:
		val = fTopfreq;
		break;
	case HB_Q:
		val = fQ;		
		break;
	case HB_WARP:
		val = fWarp;
		break;
	case HB_GAIN:
		val = fGain;		
		break;
	default:
		break;
	}
    float2string (val, text, kVstMaxParamStrLen);    
}

//-----------------------------------------------------------------------------------------
void Harmbank::getParameterLabel(VstInt32 index, char *label)
{
	switch(index){
	case HB_BOTFREQ:
		vst_strncpy (label, "Hz",kVstMaxParamStrLen);
		break;
	case HB_TOPFREQ:
		vst_strncpy (label, "Hz",kVstMaxParamStrLen);
		break;
	case HB_Q:
		vst_strncpy (label, "factor",kVstMaxParamStrLen);
		break;
	case HB_WARP:
		vst_strncpy (label, "factor",kVstMaxParamStrLen);
		break;
	case HB_GAIN:
		vst_strncpy (label, "",kVstMaxParamStrLen);
		break;
	default:
		break;
	}
	
}

// --------- Product Information ---------------

bool Harmbank::getEffectName (char* name)
{
	vst_strncpy (name, "HarmBank",kVstMaxEffectNameLen);
	return true;
}

bool Harmbank::getProductString (char* text)
{
	vst_strncpy (text, "Subharmonic Filterbank",kVstMaxProductStrLen);
	return true;
}

bool Harmbank::getVendorString (char* text)
{
	vst_strncpy (text, "MIT AudioBook(RWD)",kVstMaxVendorStrLen);
	return true;
}

// control: called before processing starts

void Harmbank::resume()
{
    double sr = updateSampleRate();
    d_srate = sr;
    filterbank_initfreqs(MAX_FILTERS, filter_freqs,filter_topfreq,fWarp);
	rbj_filterbank_init(&filterbank,filter_freqs,fQ,d_srate); 
}

#if 0
// deprecated in VST 2.4!
//-----------------------------------------------------------------------------------------
void Harmbank::process (float **inputs, float **outputs, VstInt32 sampleFrames)
{
    float *in1  =  inputs[0];
    float *out1 = outputs[0];

	double this_output;
	double this_input, this_dither;
	double cur_botfreq = fBotfreq, cur_topfreq = fTopfreq,cur_warp = fWarp, cur_Q = fQ;
	
	nfilters = filterbank_updatefreqs(MAX_FILTERS,filter_freqs,cur_botfreq,cur_topfreq,cur_warp,SEMITONE);

    while (--sampleFrames >= 0)
    {
		this_dither = (float)((( 2.0 * rand()/ (double) RAND_MAX) - 1.0) * 1.0e-10);
		this_input = (double) *in1++ + this_dither;		
		this_output	= rbj_filterbank_tick(&filterbank,filter_freqs,nfilters,this_input, cur_Q);		
		*out1++ += (float) (this_output * fGain);
    }
}
#endif
//-----------------------------------------------------------------------------------------
void Harmbank::processReplacing (float **inputs, float **outputs, VstInt32 sampleFrames)
{
    float *in1  =  inputs[0];
    float *out1 = outputs[0];
	double this_output;
	double this_input, this_dither;
	double cur_botfreq = fBotfreq, cur_topfreq = fTopfreq,cur_warp = fWarp, cur_Q = fQ;
	// save CPU by updating once per block
	nfilters = filterbank_updatefreqs(MAX_FILTERS,filter_freqs,cur_botfreq,cur_topfreq,cur_warp,SEMITONE);
    while (--sampleFrames >= 0)
    {
		this_dither = (float)((( 2.0 * rand() * InvRandMax) - 1.0) * 1.0e-10);
		this_input = (double) *in1++  + this_dither;			
		this_output	= rbj_filterbank_tick(&filterbank,filter_freqs,nfilters,this_input,cur_Q);	
		*out1++ = (float) (this_output * fGain);
    }
}
