/*
*	File:		Polynomial8.cpp
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

#include "Polynomial8.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
COMPONENT_ENTRY(Polynomial8)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Polynomial8::Polynomial8
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Polynomial8::Polynomial8(AudioUnit component)
	:	AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	
	//	Set-up our custom parameters:
	SetParameter(kWaveShapingIndex, kDefaultValue_WaveShapingIndex);
	SetParameter(kOutputLevel, kDefaultValue_OutputLevel);
	SetParameter(kMixLevel, kDefaultValue_MixLevel);
	
	for(int index = 0; index < kNumberOfCoefficients; index++)
		SetParameter(index, kDefaultValue_Coefficient);
	
	//	Upon instantiation, load up the default preset:
	AUPreset defaultPreset = {kPreset_Default, kPreset_Default};
	NewFactoryPresetSet(defaultPreset);
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Polynomial8::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Polynomial8::GetParameterInfo
(
	AudioUnitScope			inScope,
	AudioUnitParameterID	inParameterID,
	AudioUnitParameterInfo&	outParameterInfo
)
{
	OSStatus result = noErr;
	
	//	Check all parameters as readable/writable:
	outParameterInfo.flags = 
	kAudioUnitParameterFlag_IsWritable | 
	kAudioUnitParameterFlag_IsReadable;
    
	/*	Check that parameter info requests are for the audio unit's
		global scope -- see Audio Unit Programming Guide for this 
		requirement.
	 */
    if(inScope == kAudioUnitScope_Global)
	{
		/*	If the requested parameter is between 0 and 8, 
			it's a coefficient. 
			All coefficients have the same unit, minimum, maximum and
			default values.
		 */
		if(inParameterID < kNumberOfCoefficients)
		{	
			
			AUBase::FillInParameterName
			(
				outParameterInfo, 
				coefficientNameArray[inParameterID], 
				NULL
			);
			
			outParameterInfo.unit = kAudioUnitParameterUnit_LinearGain;
			outParameterInfo.minValue = kMinimumValue_Coefficient;
			outParameterInfo.maxValue = kMaximumValue_Coefficient;
			outParameterInfo.defaultValue = kDefaultValue_Coefficient;
		}
		else
		{
			/*	Parameter ID's 9 through 11 refer to:
				. Waveshaping index (9)
				. Linear output attenuator (10)
				. Wet/dry mix level (11)
			 */
			switch(inParameterID)
			{
				case kWaveShapingIndex:
					
					AUBase::FillInParameterName
					(
						outParameterInfo, 
						kParamName_WaveShapingIndex, 
						NULL
					);
					
					outParameterInfo.unit	= 
					kAudioUnitParameterUnit_LinearGain;
					
					outParameterInfo.minValue = 
					kMinimumValue_WaveShapingIndex;
					
					outParameterInfo.maxValue = 
					kMaximumValue_WaveShapingIndex;
					
					outParameterInfo.defaultValue = 
					kDefaultValue_WaveShapingIndex;
					
					break;
				
					
				case kOutputLevel:
					
					AUBase::FillInParameterName
					(
						outParameterInfo, 
						kParamName_OutputLevel, 
						NULL
					);
					
					outParameterInfo.unit = 
					kAudioUnitParameterUnit_Decibels;
					
					outParameterInfo.minValue = 
					kMinimumValue_OutputLevel;
					
					outParameterInfo.maxValue = 
					kMaximumValue_OutputLevel;
					
					outParameterInfo.defaultValue = 
					kDefaultValue_OutputLevel;
					
					break;
				
					
				case kMixLevel:
					
					AUBase::FillInParameterName
					(
						outParameterInfo, 
						kParamName_MixLevel, 
						NULL
					);
					
					outParameterInfo.unit = 
					kAudioUnitParameterUnit_LinearGain;
					
					outParameterInfo.minValue = 
					kMinimumValue_MixLevel;
					
					outParameterInfo.maxValue = 
					kMaximumValue_MixLevel;
					
					outParameterInfo.defaultValue = 
					kDefaultValue_MixLevel;
					
					break;
				
				//	An ID outside the range 0 - 11 is invalid:
				default:
					result = kAudioUnitErr_InvalidParameter;
					break;
			}
		}
	}
	else result = kAudioUnitErr_InvalidParameter;
	
	return result;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Polynomial8::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Polynomial8::GetPropertyInfo 
(
	AudioUnitPropertyID	inID,
	AudioUnitScope		inScope,
	AudioUnitElement	inElement,
	UInt32&				outDataSize,
	Boolean&			outWritable
)
{
	if(inScope == kAudioUnitScope_Global) 
	{
		/*	The only custom property that our audio unit implements
			is its Cocoa view.
		 */
		switch(inID) 
		{
			case kAudioUnitProperty_CocoaUI:
				outWritable = false;
				outDataSize = sizeof (AudioUnitCocoaViewInfo);
				return noErr;
		}
	}
	
	return AUEffectBase::GetPropertyInfo
	(
		inID, 
		inScope, 
		inElement, 
		outDataSize, 
		outWritable
	);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Polynomial8::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Polynomial8::GetProperty
(
	AudioUnitPropertyID inID,
	AudioUnitScope 		inScope,
	AudioUnitElement 	inElement,
	void*				outData 
)
{
	if(inScope == kAudioUnitScope_Global) 
	{
		//	Here we return the actual Cocoa view:
		switch(inID) 
		{
			case kAudioUnitProperty_CocoaUI:
			{
				/*	Look for a resource in the main bundle 
					by name and type.*/
				CFBundleRef bundle = 
				CFBundleGetBundleWithIdentifier
				(
					CFSTR("com.UseAndDisposeOf_.Polynomial8")
				);
				if (bundle == NULL) return fnfErr;
                
				CFURLRef bundleURL = 
				CFBundleCopyResourceURL
				( 
					bundle, 
                    CFSTR
					(
						"UseAndDisposeOf_Polynomial8_CocoaViewFactory"
					), 
                    CFSTR("bundle"), 
                    NULL
				);
                if(bundleURL == NULL) return fnfErr;
                
				CFStringRef className = CFSTR("UseAndDisposeOf_Polynomial8_CocoaViewFactory");
				
				AudioUnitCocoaViewInfo cocoaInfo = 
				{
					bundleURL, 
					{className}
				};
				*((AudioUnitCocoaViewInfo*) outData) = cocoaInfo;
				
				return noErr;
			}
		}
	}

	return AUEffectBase::GetProperty
	(
		inID, 
		inScope, 
		inElement, 
		outData
	);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Polynomial8::GetPresets
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult	Polynomial8::GetPresets(CFArrayRef* outData) const
{
	//	Create an array with all the audio unit's presets:
	if (outData == NULL) return noErr;
	
	CFMutableArrayRef presetsArray = 
	CFArrayCreateMutable 
	(
		NULL, 
		kNumberPresets, 
		NULL
	);
	
	for (int i = 0; i < kNumberPresets; ++i)
	{ 
		CFArrayAppendValue (presetsArray, &kPresets[i]);
    }
	
	//	Return the preset array to the caller:
	*outData = (CFArrayRef)presetsArray;	
	
	return noErr;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Polynomial8::NewFactoryPresetSet
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus Polynomial8::NewFactoryPresetSet 
(
	const AUPreset& inNewFactoryPreset
)
{
	SInt32 chosenPreset = inNewFactoryPreset.presetNumber;
	
	//	Cycle through all of this audio unit's presets:
	for(int i = 0; i < kNumberPresets; ++i)
	{
		/*	If the requested preset matches any of our audio unit's
			presets:
		 */
		if(chosenPreset == kPresets[i].presetNumber)
		{
			/*	Load the preset by explicitly setting all parameters
				to the appropiate values for such preset:
			 */
			switch(chosenPreset)
			{
				case kPreset_One:
					
					SetParameter
					(
						kWaveShapingIndex, 
						7.55
					);
					
					SetParameter
					(
						kOutputLevel, 
						kDefaultValue_OutputLevel
					);
					
					SetParameter(kMixLevel, 90); 
					
					SetParameter(kCoefficient0, 0); 
					SetParameter(kCoefficient1, 0); 
					SetParameter(kCoefficient2, 1); 
					SetParameter(kCoefficient3, 0); 
					SetParameter(kCoefficient4, 0); 
					SetParameter(kCoefficient5, 0); 
					SetParameter(kCoefficient6, 0); 
					SetParameter(kCoefficient7, 0); 
					SetParameter(kCoefficient8, 0); 
					
					break;
					
					
				case kPreset_Two:
					
					SetParameter
					(
						kWaveShapingIndex, 
						3.55
					);
					
					SetParameter
					(
						kOutputLevel, 
						kMaximumValue_OutputLevel
					);
					
					SetParameter(kMixLevel, 17); 
					
					SetParameter(kCoefficient0, 0); 
					SetParameter(kCoefficient1, 3.5); 
					SetParameter(kCoefficient2, 0); 
					SetParameter(kCoefficient3, 0); 
					SetParameter(kCoefficient4, 0); 
					SetParameter(kCoefficient5, 0); 
					SetParameter(kCoefficient6, 0); 
					SetParameter(kCoefficient7, 0); 
					SetParameter(kCoefficient8, 0); 
					
					break;
			}
            
            SetAFactoryPresetAsCurrent(kPresets[i]);
			
			return noErr;
		}
	}
	
	//	If no matches were found, the requested preset does not exist.
	return kAudioUnitErr_InvalidPropertyValue;
}


#pragma mark ____Polynomial8EffectKernel
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Polynomial::PolynomialKernel::PolynomialKernel()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Polynomial8::Polynomial8Kernel::Polynomial8Kernel
(
	AUEffectBase* inAudioUnit
) 
	:	AUKernelBase(inAudioUnit) 
{
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Polynomial8::Polynomial8Kernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Polynomial8::Polynomial8Kernel::Reset()
{
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	Polynomial8::Polynomial8Kernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Polynomial8::Polynomial8Kernel::Process
(	
	const Float32*	inSourceP,
	Float32*		inDestP,
	UInt32 			inFramesToProcess,
	UInt32			inNumChannels, 
	bool&			ioSilence 
)
{
	/*	The main DSP algorithm is essencially an adaptation of
		the original Csound polynomial algorithm by Anthony Kazzar.
	 */
	
	//	If this audio unit is bypassed, save the cycles by returning early.
	if(ioSilence) return;
	
	UInt32 nSampleFrames		= inFramesToProcess;
	const Float32* sourceP		= inSourceP;
	Float32* destP				= inDestP;
	Float32 waveShapingIndex	= GetParameter(kWaveShapingIndex);
	Float32 mixLevel			= GetParameter(kMixLevel) / 100.;
	Float32	wetSample			= 0.;
	Float32 drySample			= 0.;
	Float32 outputSample		= 0.;
	Float32 sum					= 0.;
	
	/*	Compute the logarithmic scaling factor that will be applied to
		the waveshaped audio:
	 */
	Float32 scalingFactor = 
	pow(10, (GetParameter(kOutputLevel) / 20.));
	
	/*	Get the current values of the coefficients in the transfer 
		function:
	*/
	for(int index = 0; index < kNumberOfCoefficients; index++)
		coefficientArray[index] = GetParameter(index);

	//	Process a chunk of audio:
	while(nSampleFrames--) 
	{
		/*	For every sample: 
			1.	Waveshape a copy of the sample.
			2.	Keep a another copy of the sample intact:
				(in case the dry/wet mix is not set to 100% wet)
		 */
		wetSample = drySample = *sourceP++;
		wetSample *= waveShapingIndex;
		
		sum = coefficientArray[8];
		
		for(int i = 7; i >= 0; --i) 
		{
			sum *= wetSample;
			sum += coefficientArray[i];
		}
		
		/*	Calculate the percentage of processed and dry output
			for this sample, according to the wet/dry mix level:
		 */
		wetSample	 = sum * mixLevel; 
		drySample	 = drySample * (1. - mixLevel);
		
		//	Scale the final output:
		outputSample = ((wetSample + drySample) * scalingFactor);
		
		//	Crudely limit the output if it's clipping: 
		if(outputSample < -0.99)
			outputSample = -0.99;
		else if(outputSample > 0.99)
			outputSample = 0.99;
		
		//	Store the resulting sample to the output buffer:
		*destP++ = outputSample;
	}
}
