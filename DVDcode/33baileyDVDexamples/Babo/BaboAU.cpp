/*
 *  BaboAU.cpp
 *  Babo
 *
 *  Created by Jonathan Bailey on 7/23/09.
 *  Copyright 2009 Drumwell Group. All rights reserved.
 *
 */


#include "BaboAU.h"
#include "AUBaseHelper.h"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

COMPONENT_ENTRY(BaboAU)


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BaboAU::BaboAU (constructor)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BaboAU::BaboAU(AudioUnit component)
	: AUBase(component, 1, 1)
{
	CreateElements();

	Globals()->UseIndexedParameters(kNumberOfParameters);
	SetParameter(kParam_RoomW, kAudioUnitScope_Global, 0, kDefaultValue_RoomW, 0);
	SetParameter(kParam_RoomD, kAudioUnitScope_Global, 0, kDefaultValue_RoomD, 0);
	SetParameter(kParam_RoomH, kAudioUnitScope_Global, 0, kDefaultValue_RoomH, 0);
	SetParameter(kParam_SourceX, kAudioUnitScope_Global, 0, kDefaultValue_SourceX, 0);
	SetParameter(kParam_SourceY, kAudioUnitScope_Global, 0, kDefaultValue_SourceY, 0);
	SetParameter(kParam_SourceZ, kAudioUnitScope_Global, 0, kDefaultValue_SourceZ, 0);
	
#if AU_DEBUG_DISPATCHER
	mDebugDispatcher = new AUDebugDispatcher (this);
#endif
	
	// set up sample rate variables
	const CAStreamBasicDescription & theDesc = GetStreamFormat(kAudioUnitScope_Output, 0);	
	sr = theDesc.mSampleRate;
	onedsr = 1.0/sr;
	
	// initialize Babo Reverb object
	BaboReverb = (BABO*) malloc(sizeof(BABO));
	set_expert_values(BaboReverb);
	
	// set up internal state variables
	room_w		= kDefaultValue_RoomW;
	room_d		= kDefaultValue_RoomD;
	room_h		= kDefaultValue_RoomH;
	
	reverb_created = false;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BaboAU::~BaboAU (destructor)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
BaboAU::~BaboAU() {
#if AU_DEBUG_DISPATCHER
	delete mDebugDispatcher;
#endif

	if (BaboReverb) free(BaboReverb);
}



ComponentResult BaboAU::Initialize()
{
	// get the current numChannels for input and output.
	// a host may test various combinations of these 
	// regardless of the outInfo returned by our SupportedNumChannels method
	SInt16 auNumInputs = (SInt16) GetInput(0)->GetStreamFormat().mChannelsPerFrame;
	SInt16 auNumOutputs = (SInt16) GetOutput(0)->GetStreamFormat().mChannelsPerFrame;
	
	if (((auNumInputs == 1) || (auNumInputs == 2)) && (auNumOutputs == 2))
	{    
		return noErr;
	}
	else
		return kAudioUnitErr_FormatNotSupported;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BaboAU::GetParameterValueStrings
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus BaboAU::GetParameterValueStrings(AudioUnitScope		inScope,
										  AudioUnitParameterID	inParameterID,
										  CFArrayRef *		outStrings)
{
        
    return kAudioUnitErr_InvalidProperty;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BaboAU::GetParameterInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus BaboAU::GetParameterInfo(AudioUnitScope		inScope,
								  AudioUnitParameterID	inParameterID,
								  AudioUnitParameterInfo	&outParameterInfo )
{
	OSStatus result = noErr;

	outParameterInfo.flags = 	kAudioUnitParameterFlag_IsWritable
						|		kAudioUnitParameterFlag_IsReadable;
    
    if (inScope == kAudioUnitScope_Global) {
        switch(inParameterID)
        {
            case kParam_RoomW:
                AUBase::FillInParameterName (outParameterInfo, kParameterRoomWName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Meters;
                outParameterInfo.minValue = kMinimumValue_RoomW;
                outParameterInfo.maxValue = kMaximumValue_RoomW;
                outParameterInfo.defaultValue = kDefaultValue_RoomW;
                break;
            case kParam_RoomD:
                AUBase::FillInParameterName (outParameterInfo, kParameterRoomDName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Meters;
                outParameterInfo.minValue = kMinimumValue_RoomD;
                outParameterInfo.maxValue = kMaximumValue_RoomD;
                outParameterInfo.defaultValue = kDefaultValue_RoomD;
                break;
            case kParam_RoomH:
                AUBase::FillInParameterName (outParameterInfo, kParameterRoomHName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Meters;
                outParameterInfo.minValue = kMinimumValue_RoomH;
                outParameterInfo.maxValue = kMaximumValue_RoomH;
                outParameterInfo.defaultValue = kDefaultValue_RoomH;
                break;
            case kParam_SourceX:
                AUBase::FillInParameterName (outParameterInfo, kParameterSourceXName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Meters;
                outParameterInfo.minValue = kMinimumValue_SourceX;
                outParameterInfo.maxValue = kMaximumValue_SourceX;
                outParameterInfo.defaultValue = kDefaultValue_SourceX;
                break;
            case kParam_SourceY:
                AUBase::FillInParameterName (outParameterInfo, kParameterSourceYName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Meters;
                outParameterInfo.minValue = kMinimumValue_SourceY;
                outParameterInfo.maxValue = kMaximumValue_SourceY;
                outParameterInfo.defaultValue = kDefaultValue_SourceY;
                break;
            case kParam_SourceZ:
                AUBase::FillInParameterName (outParameterInfo, kParameterSourceZName, false);
                outParameterInfo.unit = kAudioUnitParameterUnit_Meters;
                outParameterInfo.minValue = kMinimumValue_SourceZ;
                outParameterInfo.maxValue = kMaximumValue_SourceZ;
                outParameterInfo.defaultValue = kDefaultValue_SourceZ;
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
//	BaboAU::GetPropertyInfo
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus BaboAU::GetPropertyInfo (	AudioUnitPropertyID	inID,
									AudioUnitScope		inScope,
									AudioUnitElement	inElement,
									UInt32 &		outDataSize,
									Boolean &		outWritable)
{
	return AUBase::GetPropertyInfo (inID, inScope, inElement, outDataSize, outWritable);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BaboAU::GetProperty
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
OSStatus BaboAU::GetProperty(	AudioUnitPropertyID inID,
								AudioUnitScope 		inScope,
								AudioUnitElement 	inElement,
								void *			outData )
{
	return AUBase::GetProperty (inID, inScope, inElement, outData);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BaboAU::SupportedNumChannels
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

UInt32 BaboAU::SupportedNumChannels (const AUChannelInfo** outInfo)
{
	static const AUChannelInfo sChannels[2] = { {1, 2}, {2, 2} };
	if (outInfo) *outInfo = sChannels;
	return sizeof (sChannels) / sizeof (AUChannelInfo);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BaboAU::Render
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

ComponentResult BaboAU::Render(AudioUnitRenderActionFlags &ioActionFlags,
							   const AudioTimeStamp &		inTimeStamp,
							   UInt32						nFrames)
{		
	// set up i/o pointers
	AUInputElement *input = GetInput(0);
    AUOutputElement *output = GetOutput(0);
	
	// zero the buffer before we start processing
	AUBufferList::ZeroBuffer(output->GetBufferList());
	
    ComponentResult result = PullInput(0, ioActionFlags, inTimeStamp, nFrames);
    if (result) return result;
	
	BaboReverb->input		= (float*) input->GetChannelData(0);
	BaboReverb->outleft		= (float*) output->GetChannelData(0);								
	BaboReverb->outright	= (float*) output->GetChannelData(1);
	
	float rw = Globals()->GetParameter( kParam_RoomW );
	float rd = Globals()->GetParameter( kParam_RoomD );
	float rh = Globals()->GetParameter( kParam_RoomH );
	
	// if the room dimensions have changed or we have not created the babo object yet, call the setup function 
	if(rw != room_w || rd != room_d || rh != room_h || !reverb_created) {
		room_w = BaboReverb->lx = rw;
		room_d = BaboReverb->ly = rd;
		room_h = BaboReverb->lz = rh;
		
		baboset(BaboReverb, sr, onedsr);		
		reverb_created = true;
	}
	
	// get values of source params
	BaboReverb->ksource_x	= Globals()->GetParameter( kParam_SourceX );
	BaboReverb->ksource_y	= Globals()->GetParameter( kParam_SourceY );
	BaboReverb->ksource_z	= Globals()->GetParameter( kParam_SourceZ );
	
	// perform calculations
	babo(BaboReverb, sr, onedsr, nFrames);
	
	return noErr;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BaboAU::StreamFormatWritable
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool BaboAU::StreamFormatWritable(AudioUnitScope			scope,
								   AudioUnitElement			element)
{
	return IsInitialized() ? false : true;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//	BaboAU::GetTailTime
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Float64 BaboAU::GetTailTime() {
	Float64 reverb_time = (Float64)babo_reverb_time(BaboReverb);
	return reverb_time;
}



