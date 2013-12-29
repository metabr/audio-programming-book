 /*
 *	File:		OscilUnit.cpp
 *	
 *	Version:	1.0
 * 
 *	Created:	9/11/06
 *	
 *	Copyright:  Copyright © 2006 __MyCompanyName__, All Rights Reserved
 * 
 *	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc. ("Apple") in 
 *				consideration of your agreement to the following terms, and your use, installation, modification 
 *				or redistribution of this Apple software constitutes acceptance of these terms.  If you do 
 *				not agree with these terms, please do not use, install, modify or redistribute this Apple 
 *				software.
 *
 *				In consideration of your agreement to abide by the following terms, and subject to these terms, 
 *				Apple grants you a personal, non-exclusive license, under Apple's copyrights in this 
 *				original Apple software (the "Apple Software"), to use, reproduce, modify and redistribute the 
 *				Apple Software, with or without modifications, in source and/or binary forms; provided that if you 
 *				redistribute the Apple Software in its entirety and without modifications, you must retain this 
 *				notice and the following text and disclaimers in all such redistributions of the Apple Software. 
 *				Neither the name, trademarks, service marks or logos of Apple Computer, Inc. may be used to 
 *				endorse or promote products derived from the Apple Software without specific prior written 
 *				permission from Apple.  Except as expressly stated in this notice, no other rights or 
 *				licenses, express or implied, are granted by Apple herein, including but not limited to any 
 *				patent rights that may be infringed by your derivative works or by other works in which the 
 *				Apple Software may be incorporated.
 *
 *				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO WARRANTIES, EXPRESS OR 
 *				IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY 
 *				AND FITNESS FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE 
 *				OR IN COMBINATION WITH YOUR PRODUCTS.
 *
 *				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR CONSEQUENTIAL 
 *				DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 *				OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, 
 *				REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER 
 *				UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN 
 *				IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*=============================================================================
	OscilUnit.cpp
	
 =============================================================================*/

/*
	This is an example implementation of a sin wave synth using AUInstrumentBase classes
	
	It illustrates a basic usage of these classes
	
	It artificially limits the number of notes at one time to 12, so the note-stealing 
	algorithm is used - you should know how this works!
	
	Most of the work you need to do is defining a Note class (see TestNote). AUInstrument manages the
	creation and destruction of notes, the various stages of a note's lifetime.
	
	The project also defines CA_AUTO_MIDI_MAP (OTHER_C_FLAGS). This adds all the code that is needed
	to map MIDI messages to specific parameter changes. This can be seen in AU Lab's MIDI Editor window
	CA_AUTO_MIDI_MAP is implemented in AUMIDIBase.cpp/.h
*/


#include "OscilUnit.h"

COMPONENT_ENTRY(OscilUnit)

#pragma mark OscilUnit Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	OscilUnit::OscilUnit
//
// This synth has No inputs, One output
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OscilUnit::OscilUnit(ComponentInstance inComponentInstance)
	: AUMonotimbralInstrumentBase(inComponentInstance, 0, 1)
{
	CreateElements();
	
	Globals()->UseIndexedParameters(kNumberOfParameters); 
	Globals()->SetParameter(kParameter_Wavetable,kDefaultValue_Wavetable);
	
	// sine wave table
	for(int i = 0; i < kWaveArraySize; i++) {
		mSine[i] = (sin(i*twopi/kWaveArraySize)+1)/2;
	}
	
	// sawtooth wave table      
	for(int i = 0; i < kWaveArraySize; i++) {
		double point = 0.0;
		for(int j = 1; j <= kHarmonics; j++)
			point += sin((i*twopi/kWaveArraySize)*j)/j;
		mSawtooth[i] = (point+2)/4;
	}	
	
	// square wave table      
	for(int i = 0; i < kWaveArraySize; i++) {
		double point = 0.0;
		for(int j = 1; j <= kHarmonics; j += 2)
			point += sin((i*twopi/kWaveArraySize)*j)/j;
		mSquare[i] = (point+1)/2;
	}			
	
	
	// triangle wave table - phase shifted to start (basically) at zero
	for(int i = 0; i < kWaveArraySize; i++) {
		double point = 0.0;
		for(int j = 1; j <= kHarmonics; j += 2)
			point += sin(((i*twopi/kWaveArraySize)+1.5*pi)*j)/(j*j);		
		mTriangle[i] = (point+1)/2;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	OscilUnit::Initialize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus OscilUnit::Initialize()
{	
	AUMonotimbralInstrumentBase::Initialize();	
	SetNotes(kNumNotes, kMaxActiveNotes, mTestNotes, sizeof(TestNote));
	
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	OscilUnit::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult OscilUnit::GetParameterInfo(
											AudioUnitScope inScope,
											AudioUnitParameterID inParameterID,
											AudioUnitParameterInfo &outParameterInfo)
{
	if (inParameterID != kParameter_Wavetable) return kAudioUnitErr_InvalidParameter;
	if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;
	
	outParameterInfo.flags = kAudioUnitParameterFlag_IsWritable |
	kAudioUnitParameterFlag_IsReadable;
	
	AUBase::FillInParameterName (outParameterInfo, kParamName_Wavetable, false);
	
	outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
	outParameterInfo.minValue = kSineWave_Wavetable;
	outParameterInfo.maxValue = kTriangleWave_Wavetable;
	outParameterInfo.defaultValue = kDefaultValue_Wavetable;
	
	return noErr;
}

ComponentResult OscilUnit::GetParameterValueStrings (AudioUnitScope inScope, 
													 AudioUnitParameterID inParameterID, 
													 CFArrayRef *outStrings) 
{
	
	if ((inScope == kAudioUnitScope_Global) 
		&& (inParameterID == kParameter_Wavetable)) {
		if (outStrings == NULL) return noErr;
		
		CFStringRef strings [] = {
			kMenuItem_Wavetable_Sine,
			kMenuItem_Wavetable_Square, 
			kMenuItem_Wavetable_Sawtooth, 	
			kMenuItem_Wavetable_Triangle};
		
        *outStrings = CFArrayCreate (NULL,(const void **) strings,(sizeof (strings) / sizeof (strings [0])),NULL);
        return noErr;
		
    }
    return kAudioUnitErr_InvalidParameter;
}


#pragma mark TestNote Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	OscilUnit::Render
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus TestNote::Render(
							   UInt32 inNumFrames, 
							   AudioBufferList& inBufferList)
{
	float *left, *right;
	
	int numChans = inBufferList.mNumberBuffers;
	if (numChans > 2) return -1;
	
	left = (float*)inBufferList.mBuffers[0].mData;
	right = numChans == 2 ? (float*)inBufferList.mBuffers[1].mData : 0;
	
	
	// modifications to implement oscaa
	
	// figure out which wave table to use
	int wavetable = (int) GetGlobalParameter (kParameter_Wavetable);
	switch(wavetable) {
		case kSineWave_Wavetable: waveArrayPointer = mSine; break;
		case kSquareWave_Wavetable: waveArrayPointer = mSquare; break;
		case kSawtoothWave_Wavetable: waveArrayPointer = mSawtooth; break;
		case kTriangleWave_Wavetable: waveArrayPointer = mTriangle; break;
		default: waveArrayPointer = mSine; break;
	}
	
	// obtain frequency and sample rate
	double freq = Frequency();
	double sampleRate = SampleRate();
	
	// compute max matthews table increment 
	long inc = freq * kWaveArraySize / sampleRate;
	
	switch (GetState())
	{
		case kNoteState_Attacked :
		case kNoteState_Sostenutoed :
		case kNoteState_ReleasedButSostenutoed :
		case kNoteState_ReleasedButSustained :
		{
			for (UInt32 frame=0; frame<inNumFrames; ++frame)
			{
				if (amp < maxamp) amp += up_slope;
				float tablevalue = waveArrayPointer[phase];
				float out = pow5(tablevalue) * amp;
				phase += inc;
				phase &= PHMASK; 
				
				left[frame] += out;
				if (right) right[frame] += out;
			}
		}
			break;
			
		case kNoteState_Released :
		{
			UInt32 endFrame = 0xFFFFFFFF;
			for (UInt32 frame=0; frame<inNumFrames; ++frame)
			{
				if (amp > 0.0) amp += dn_slope;
				else if (endFrame == 0xFFFFFFFF) endFrame = frame;
				float tablevalue = waveArrayPointer[phase];
				float out = pow5(tablevalue) * amp;
				phase += inc;
				phase &= PHMASK; 
				left[frame] += out;
				if (right) right[frame] += out;
			}
			if (endFrame != 0xFFFFFFFF) {
				NoteEnded(endFrame);
			}
		}
			break;
			
		case kNoteState_FastReleased :
		{
			UInt32 endFrame = 0xFFFFFFFF;
			for (UInt32 frame=0; frame<inNumFrames; ++frame)
			{
				if (amp > 0.0) amp += fast_dn_slope;
				else if (endFrame == 0xFFFFFFFF) endFrame = frame;
				float tablevalue = waveArrayPointer[phase];
				float out = pow5(tablevalue) * amp;
				phase += inc;
				phase &= PHMASK; 
				left[frame] += out;
				if (right) right[frame] += out;
			}
			if (endFrame != 0xFFFFFFFF) {
				NoteEnded(endFrame);
			}
		}
			break;
		default :
			break;
	}
	return noErr;
}


