/*
*	File:		«PROJECTNAME».h
*	
*	Version:	1.0
* 
*	Created:	«DATE»
*	
*	Copyright:  Copyright © «YEAR» «ORGANIZATIONNAME», All Rights Reserved
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
 
#ifndef __«PROJECTNAME»_h__
#define __«PROJECTNAME»_h__
 
#include "AUInstrumentBase.h"
#include "«PROJECTNAME»Version.h"

#define DEBUG_PRINT 1

// 
static const UInt32 kNumNotes = 12;
static const UInt32 kMaxActiveNotes = 8;

static const AudioUnitParameterID kGlobalVolumeParam = 0;
static const CFStringRef kGlobalVolumeName = CFSTR("global volume");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const double twopi = 2.0 * 3.14159265358979;

inline double pow5(double x) { double x2 = x*x; return x2*x2*x; }


struct TestNote : public SynthNote
{
	virtual					~TestNote() {}

	virtual void			Attack(const MusicDeviceNoteParams &inParams)
								{ 
#if DEBUG_PRINT
									printf("TestNote::Attack %08X %d\n", this, GetState());
#endif
									double sampleRate = SampleRate();
									phase = 0.;
									amp = 0.;
									maxamp = 0.4 * pow(inParams.mVelocity/127., 3.); 
									up_slope = maxamp / (0.1 * sampleRate);
									dn_slope = -maxamp / (0.9 * sampleRate);
									fast_dn_slope = -maxamp / (0.005 * sampleRate);
								}
	virtual void			Kill(UInt32 inFrame); // voice is being stolen.
	virtual void			Release(UInt32 inFrame);
	virtual void			FastRelease(UInt32 inFrame);
	virtual Float32			Amplitude() { return amp; } // used for finding quietest note for voice stealing.
	virtual OSStatus		Render(UInt32 inNumFrames, AudioBufferList& inBufferList);

	double phase, amp, maxamp;
	double up_slope, dn_slope, fast_dn_slope;
};

class «PROJECTNAME» : public AUMonotimbralInstrumentBase
{
public:
								«PROJECTNAME»(ComponentInstance inComponentInstance);
	virtual						~«PROJECTNAME»();
				
	virtual ComponentResult		Initialize();
	virtual void				Cleanup();
	virtual ComponentResult		Version() { return k«PROJECTNAME»Version; }

	virtual ComponentResult		GetParameterInfo(		AudioUnitScope					inScope,
														AudioUnitParameterID			inParameterID,
														AudioUnitParameterInfo &		outParameterInfo);
private:
	
	TestNote mTestNotes[kNumNotes];
};

#endif // __«PROJECTNAME»_h__

