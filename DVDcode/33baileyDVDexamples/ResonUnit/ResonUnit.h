/*
 *	File:		ResonUnit.h
 *	
 *	Version:	1.0
 * 
 *	Created:	1/6/08
 *	
 *	Copyright:  Copyright © 2008 Drumwell Group Inc., All Rights Reserved
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
#include "AUEffectBase.h"
#include "ResonUnitVersion.h"

#if AU_DEBUG_DISPATCHER
#include "AUDebugDispatcher.h"
#endif


#ifndef __ResonUnit_h__
#define __ResonUnit_h__

// global variables needed in signal processing calculations
const double pi = 3.14159265358979;
const double twopi = 2.0 * pi;

// parameters
#pragma mark ____ResonUnit Parameters

// center frequency 
static CFStringRef kParamName_CenterFrequency = CFSTR ("Center Frequency"); 
static const float kMinimumValue_CenterFrequency = 20.0;
static const float kMaximumValue_CenterFrequency = 20000.0;
static const float kDefaultValue_CenterFrequency = 4000.0;

// bandwidth
static CFStringRef kParamName_BandWidth = CFSTR ("Bandwidth"); 
static const float kMinimumValue_Bandwidth = 1.0;
static const float kMaximumValue_Bandwidth = 10000.0;
static const float kDefaultValue_Bandwidth = 100.0;

// scale
static CFStringRef kParamName_Scale = CFSTR ("Scale");
static const int kMinimumValue_Scale = 0;
static const int kMaximumValue_Scale = 2;
static const int kDefaultValue_Scale = 0;

// menu item names for the scale parameter
static CFStringRef kMenuItem_Zero = CFSTR ("0");
static CFStringRef kMenuItem_One = CFSTR ("1");
static CFStringRef kMenuItem_Two = CFSTR ("2");

// parameter identifiers
enum {
    kParameter_CenterFrequency  = 0,
    kParameter_Bandwidth      = 1,
    kParameter_Scale   = 2,
    kNumberOfParameters   = 3
};


#pragma mark ____ResonUnit
class ResonUnit : public AUEffectBase
{
public:
	ResonUnit(AudioUnit component);
#if AU_DEBUG_DISPATCHER
	virtual ~ResonUnit () { delete mDebugDispatcher; }
#endif
	
	virtual AUKernelBase *		NewKernel() { return new ResonUnitKernel(this); }
	
	virtual	ComponentResult		GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	ComponentResult		GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual ComponentResult		GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual ComponentResult		GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);
	
 	virtual	bool				SupportsTail () { return false; }
	
	/*! @method Version */
	virtual ComponentResult	Version() { return kResonUnitVersion; }
	
    
	
protected:
		class ResonUnitKernel : public AUKernelBase		
		// most of the real work happens here
	{
public:
		
		ResonUnitKernel(AUEffectBase *inAudioUnit );
		
		// *Required* overides for the process method for this effect
		// processes one channel of interleaved samples
		virtual void Process(	const Float32 *inSourceP,								Float32 *inDestP,
								UInt32 inFramesToProcess,
								UInt32 inNumChannels,
								bool &ioSilence);
		
		virtual void		Reset();
		
private: 
			
		// variables from RESON structure
		float	kcf, kbw;
		int		scale;
		double	c1, c2, c3, yt1, yt2, cosf, prvcf, prvbw;
		
		// variables from CSOUND structure
		float 	tpidsr, mtpdsr; 
		
		// useful variable
		Float32  sampleRate; 
		
	};
	
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif