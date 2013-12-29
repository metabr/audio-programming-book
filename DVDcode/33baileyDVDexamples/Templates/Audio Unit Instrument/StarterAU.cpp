/*
*	File:		ÇPROJECTNAMEÈ.h
*	
*	Version:	1.0
* 
*	Created:	ÇDATEÈ
*	
*	Copyright:  Copyright © ÇYEARÈ ÇORGANIZATIONNAMEÈ, All Rights Reserved
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
	ÇPROJECTNAMEÈ.h
	
=============================================================================*/

#include "ÇPROJECTNAMEÈ.h"

#pragma mark ÇPROJECTNAMEÈ Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ÇPROJECTNAMEÈ)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ÇPROJECTNAMEÈ::ÇPROJECTNAMEÈ
//
// This synth has No inputs, One output
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ÇPROJECTNAMEÈ::ÇPROJECTNAMEÈ(ComponentInstance inComponentInstance)
	: AUMonotimbralInstrumentBase(inComponentInstance, 0, 1)
{
	CreateElements();
	
	Globals()->UseIndexedParameters(1); // we're only defining one param
	Globals()->SetParameter (kGlobalVolumeParam, 1.0);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ÇPROJECTNAMEÈ::~ÇPROJECTNAMEÈ
//
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ÇPROJECTNAMEÈ::~ÇPROJECTNAMEÈ()
{
}


void ÇPROJECTNAMEÈ::Cleanup()
{
#if DEBUG_PRINT
	printf("ÇPROJECTNAMEÈ::Cleanup\n");
#endif
}

ComponentResult ÇPROJECTNAMEÈ::Initialize()
{	
#if DEBUG_PRINT
	printf("->ÇPROJECTNAMEÈ::Initialize\n");
#endif
	AUMonotimbralInstrumentBase::Initialize();
	
	SetNotes(kNumNotes, kMaxActiveNotes, mTestNotes, sizeof(TestNote));
#if DEBUG_PRINT
	printf("<-ÇPROJECTNAMEÈ::Initialize\n");
#endif
	
	return noErr;
}

ComponentResult		ÇPROJECTNAMEÈ::GetParameterInfo(		AudioUnitScope					inScope,
														AudioUnitParameterID			inParameterID,
														AudioUnitParameterInfo &		outParameterInfo)
{
	if (inParameterID != kGlobalVolumeParam) return kAudioUnitErr_InvalidParameter;
	if (inScope != kAudioUnitScope_Global) return kAudioUnitErr_InvalidScope;

	outParameterInfo.flags = SetAudioUnitParameterDisplayType (0, kAudioUnitParameterFlag_DisplaySquareRoot);
    outParameterInfo.flags += kAudioUnitParameterFlag_IsWritable;
	outParameterInfo.flags += kAudioUnitParameterFlag_IsReadable;

	AUBase::FillInParameterName (outParameterInfo, kGlobalVolumeName, false);
	outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
	outParameterInfo.minValue = 0;
	outParameterInfo.maxValue = 1.0;
	outParameterInfo.defaultValue = 1.0;
	return noErr;
}


#pragma mark TestNote Methods

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void			TestNote::Release(UInt32 inFrame)
{
	SynthNote::Release(inFrame);
#if DEBUG_PRINT
	printf("TestNote::Release %08X %d\n", this, GetState());
#endif
}

void			TestNote::FastRelease(UInt32 inFrame) // voice is being stolen.
{
	SynthNote::Release(inFrame);
#if DEBUG_PRINT
	printf("TestNote::Release %08X %d\n", this, GetState());
#endif
}

void			TestNote::Kill(UInt32 inFrame) // voice is being stolen.
{
	SynthNote::Kill(inFrame);
#if DEBUG_PRINT
	printf("TestNote::Kill %08X %d\n", this, GetState());
#endif
}

OSStatus		TestNote::Render(UInt32 inNumFrames, AudioBufferList& inBufferList)
{
	float *left, *right;
/* ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~	
	Changes to this parameter (kGlobalVolumeParam) are not being de-zippered; 
	Left as an exercise for the reader
 ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ */
	float globalVol = GetGlobalParameter(kGlobalVolumeParam);
	
	int numChans = inBufferList.mNumberBuffers;
	if (numChans > 2) return -1;
	
	left = (float*)inBufferList.mBuffers[0].mData;
	right = numChans == 2 ? (float*)inBufferList.mBuffers[1].mData : 0;

	double sampleRate = SampleRate();
	double freq = Frequency() * (twopi/sampleRate);

	
#if DEBUG_PRINT_RENDER
	printf("TestNote::Render %08X %d %g %g\n", this, GetState(), phase, amp);
#endif
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
					float out = pow5(sin(phase)) * amp * globalVol;
					phase += freq;
					if (phase > twopi) phase -= twopi;
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
					float out = pow5(sin(phase)) * amp * globalVol;
					phase += freq;
					left[frame] += out;
					if (right) right[frame] += out;
				}
				if (endFrame != 0xFFFFFFFF) {
#if DEBUG_PRINT
					printf("TestNote::NoteEnded  %08X %d %g %g\n", this, GetState(), phase, amp);
#endif
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
					float out = pow5(sin(phase)) * amp * globalVol;
					phase += freq;
					left[frame] += out;
					if (right) right[frame] += out;
				}
				if (endFrame != 0xFFFFFFFF) {
#if DEBUG_PRINT
					printf("TestNote::NoteEnded  %08X %d %g %g\n", this, GetState(), phase, amp);
#endif
					NoteEnded(endFrame);
				}
			}
			break;
		default :
			break;
	}
	return noErr;
}

