/*
*	File:		Polynomial8.h
*	
*	Version:	1.0
* 
*	Created:	8/25/09 by Federico G. Saldarini
*	
*	Copyright:  Copyright © 2009 UseAndDisposeOf_, All Rights Reserved
* 
*	Contact:	fsaldarini@useanddisposeof.com
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
#include "Polynomial8Version.h"

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __Polynomial8_h__
#define __Polynomial8_h__


#pragma mark ____Polynomial8 Parameters
//	Declare all parameters for this audio unit:

//	Waveshaping Index:
static CFStringRef kParamName_WaveShapingIndex = CFSTR("WaveShaping Index");  
static const float kDefaultValue_WaveShapingIndex = 1.;  
static const float kMinimumValue_WaveShapingIndex = 0.;  
static const float kMaximumValue_WaveShapingIndex = 20.; 

//	Linear ouput attenuator:
static CFStringRef kParamName_OutputLevel = CFSTR ("Output Level");  
static const float kDefaultValue_OutputLevel = 0.;  
static const float kMinimumValue_OutputLevel = -40.;  
static const float kMaximumValue_OutputLevel = 0.; 

//	Wet/dry mix:
static CFStringRef kParamName_MixLevel = CFSTR ("Wet/Dry Mix");  
static const float kDefaultValue_MixLevel = 50.;  
static const float kMinimumValue_MixLevel = 0.;  
static const float kMaximumValue_MixLevel = 100.; 

//	An array of coefficients:
#define kNumberOfCoefficients 9
static const CFStringRef coefficientNameArray[kNumberOfCoefficients] =
{
	CFSTR ("Coefficient 0"), 
	CFSTR ("Coefficient 1"),
	CFSTR ("Coefficient 2"),
	CFSTR ("Coefficient 3"),
	CFSTR ("Coefficient 4"),
	CFSTR ("Coefficient 5"),
	CFSTR ("Coefficient 6"),
	CFSTR ("Coefficient 7"),
	CFSTR ("Coefficient 8")
};  

static const float kDefaultValue_Coefficient = 0.;  
static const float kMinimumValue_Coefficient = -20.;
static const float kMaximumValue_Coefficient = 20.;

/*	Enums for all parameters. Makes code that refers to
	parameters easier to read.
 */
enum 
{
	kCoefficient0,
	kCoefficient1,
	kCoefficient2,
	kCoefficient3,
	kCoefficient4,
	kCoefficient5,
	kCoefficient6,
	kCoefficient7,
	kCoefficient8,	
	kWaveShapingIndex,
	kOutputLevel,
	kMixLevel,
	
	kNumberOfParameters = 12
};


#pragma mark ____Polynomial Presets
//	Two simple presets:

enum
{
	kPreset_One		= 0,
	kPreset_Two		= 1,
	kNumberPresets	= 2
};

static AUPreset kPresets[kNumberPresets] = 
{
	{kPreset_One, CFSTR("Quadratic function")}, 
	{kPreset_Two, CFSTR("Saturation")}
};

static const int kPreset_Default = kPreset_One;


#pragma mark ____Polynomial8

class Polynomial8 : public AUEffectBase
{
	
public:
	
	Polynomial8(AudioUnit component);

#if AU_DEBUG_DISPATCHER
	virtual ~Polynomial8() {delete mDebugDispatcher;}
#endif
	
	virtual AUKernelBase* NewKernel() 
	{
		return new Polynomial8Kernel(this);
	}
	
	virtual	OSStatus GetParameterInfo
	(
		AudioUnitScope			inScope,
		AudioUnitParameterID	inParameterID,
		AudioUnitParameterInfo&	outParameterInfo
	);
    
	virtual OSStatus GetPropertyInfo
	(
		AudioUnitPropertyID	inID,
		AudioUnitScope		inScope,
		AudioUnitElement	inElement,
		UInt32&				outDataSize,
		Boolean&			outWritable
	);
	
	virtual OSStatus GetProperty
	(
		AudioUnitPropertyID inID,
		AudioUnitScope 		inScope,
		AudioUnitElement 	inElement,
		void*				outData
	);
	
	/*	THIS METHOD WAS ADDED TO SUPPORT PRESETS.
		Returns an array with any presets available for this
		audio unit.
	 */
	virtual ComponentResult GetPresets
	(
		CFArrayRef* outData
	) const;    
	
	/*	THIS METHOD WAS ADDED TO SUPPORT PRESETS.
		Loads a preset into the audio unit.
	 */
	virtual OSStatus NewFactoryPresetSet
	(
		const AUPreset& inNewFactoryPreset
	);
 	
	virtual	bool SupportsTail() 
	{
		return false;
	}
	
	/*! @method Version */
	virtual OSStatus Version() 
	{
		return kPolynomial8Version;
	}
	
	
protected:
	
	class Polynomial8Kernel : public AUKernelBase
	{
	public:
		Polynomial8Kernel(AUEffectBase* inAudioUnit);
	
		// *Required* overides for the process method for this effect
		// processes one channel of interleaved samples
        virtual void Process
		(	
			const Float32*	inSourceP,
			Float32*		inDestP,
			UInt32 			inFramesToProcess,
			UInt32			inNumChannels,
			bool&			ioSilence
		);
		
        virtual void Reset();
		
	private:
		//	The kernel keeps its own array of coefficients.
		Float32 coefficientArray[kNumberOfCoefficients];
	};
};

#endif