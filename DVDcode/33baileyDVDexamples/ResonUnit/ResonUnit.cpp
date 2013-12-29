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
/*=============================================================================
ResonUnit.h

=============================================================================*/
#include "ResonUnit.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(ResonUnit)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResonUnit::ResonUnit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ResonUnit::ResonUnit(AudioUnit component)
: AUEffectBase(component)
{
	CreateElements();
	Globals()->UseIndexedParameters(kNumberOfParameters);
	
	SetParameter(kParameter_CenterFrequency, kDefaultValue_CenterFrequency);
	SetParameter(kParameter_Bandwidth, kDefaultValue_Bandwidth);
	SetParameter(kParameter_Scale, kDefaultValue_Scale);
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResonUnit::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ResonUnit::GetParameterValueStrings(
													AudioUnitScope inScope,
													AudioUnitParameterID inParameterID,
													CFArrayRef *outStrings)
{
	if ((inScope == kAudioUnitScope_Global) && (inParameterID == kParameter_Scale)) {
        if (outStrings == NULL) return noErr;
        CFStringRef strings [] = { kMenuItem_Zero, kMenuItem_One, kMenuItem_Two };
        *outStrings = CFArrayCreate (NULL, (const void **) strings, (sizeof (strings) / sizeof (strings [0])), NULL);
        return noErr;
	}
	
    return kAudioUnitErr_InvalidProperty;
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResonUnit::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult ResonUnit::GetParameterInfo(
											AudioUnitScope inScope, 
											AudioUnitParameterID inParameterID, 
											AudioUnitParameterInfo	&outParameterInfo )
{
	ComponentResult result = noErr;
	
	outParameterInfo.flags = kAudioUnitParameterFlag_IsWritable | kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParameter_CenterFrequency:
				AUBase::FillInParameterName (outParameterInfo,kParamName_CenterFrequency, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Hertz;
                outParameterInfo.minValue = kMinimumValue_CenterFrequency;
                outParameterInfo.maxValue = kMaximumValue_CenterFrequency;
                outParameterInfo.defaultValue = kDefaultValue_CenterFrequency;
                break;
            case kParameter_Bandwidth:
                AUBase::FillInParameterName (outParameterInfo, kParamName_BandWidth, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Hertz;
                outParameterInfo.minValue = kMinimumValue_Bandwidth;
                outParameterInfo.maxValue = kMaximumValue_Bandwidth;
                outParameterInfo.defaultValue = kDefaultValue_Bandwidth;
                break;
            case kParameter_Scale:
                AUBase::FillInParameterName (outParameterInfo, kParamName_Scale, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Indexed;
                outParameterInfo.minValue = kMinimumValue_Scale;
                outParameterInfo.maxValue = kMaximumValue_Scale;
                outParameterInfo.defaultValue = kDefaultValue_Scale;
                break;
            default:
                result = kAudioUnitErr_InvalidParameter;
                break;
		}
	} else {
        result = kAudioUnitErr_InvalidParameter;
    }
    
	return result;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResonUnit::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ResonUnit::GetPropertyInfo (AudioUnitPropertyID	inID,
												AudioUnitScope		inScope,
												AudioUnitElement	inElement,
												UInt32 &		outDataSize,
												Boolean &		outWritable)
{
	return AUEffectBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResonUnit::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComponentResult		ResonUnit::GetProperty(	AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 	inElement,
											void *			outData )
{
	return AUEffectBase::GetProperty (inID, inScope, inElement, outData);
}


#pragma mark ____ResonUnitEffectKernel

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResonUnitKernel::ResonUnitKernel
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ResonUnit::ResonUnitKernel::ResonUnitKernel (AUEffectBase *inAudioUnit) : 
AUKernelBase (inAudioUnit) 
{
	prvcf = prvbw = -100.0;
	yt1 = yt2 = 0.0;
	
	sampleRate = GetSampleRate ();
	
	tpidsr = (float) twopi / sampleRate;
	mtpdsr = (float) -tpidsr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResonUnit::ResonUnitKernel::Reset()
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ResonUnit::ResonUnitKernel::Reset()
{
	// like the constructor
	prvcf = prvbw = -100.0;
	yt1 = yt2 = 0.0;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	ResonUnit::ResonUnitKernel::Process
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ResonUnit::ResonUnitKernel::Process(
										 const Float32 *inSourceP,
										 Float32 *inDestP,
										 UInt32 inFramesToProcess,
										 UInt32 inNumChannels, 
										 bool &ioSilence )
{
	
	if (!ioSilence) {    
		
		uint flag = 0, n;
		double      c3p1, c3t4, omc3, c2sqr;
		double      yt1, yt2;
		
		// pull out parameter values
		kcf = (float) GetParameter(kParameter_CenterFrequency);
		kbw = (float) GetParameter(kParameter_Bandwidth);
		scale = (int) GetParameter(kParameter_Scale);
		
		if (kcf != (float) prvcf) {
			prvcf = (double) kcf;
			cosf = cos(prvcf * (double)( tpidsr));
			flag = 1;                 /* Mark as changed */
		}
		if (kbw != (float)prvbw) {
			prvbw = (double) kbw;
			c3 = exp(prvbw * (double)( mtpdsr));
			flag = 1;                /* Mark as changed */
		}
		if (flag) {
			
			c3p1 = c3 + 1.0;
			c3t4 = c3 * 4.0;
			omc3 = 1.0 - c3;
			c2 = c3t4 * cosf / c3p1;               /* -B, so + below */
			c2sqr = c2 * c2;
			if (scale == 1)
				c1 = omc3 * sqrt(1.0 - c2sqr / c3t4);
			else if (scale == 2)
				c1 = sqrt((c3p1*c3p1-c2sqr) * omc3/c3p1);
			else c1 = 1.0;
		}
		
		for (n=0; n<inFramesToProcess; n++) {
			double yt0 = c1 * (double)(inSourceP[n]) + c2 * yt1 - c3 * yt2;
			inDestP[n] = (float)yt0;
			yt2 = yt1;
			yt1 = yt0;
		}
	}
}



