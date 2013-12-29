/*
 *  BaboAU.h
 *  Babo
 *
 *  Created by Jonathan Bailey on 7/23/09.
 *  Copyright 2009 Drumwell Group. All rights reserved.
 *
 */

#include "AUBase.h"
#include "BaboVersion.h"

// JB: have to declare this as extern C because we are calling C functions from a CPP file
extern "C"
{
#include "BaboReverb.h"
}

#if AU_DEBUG_DISPATCHER
	#include "AUDebugDispatcher.h"
#endif


#ifndef __BaboAU_h__
#define __BaboAU_h__


#pragma mark ____Babo Parameters


// parameters
static CFStringRef kParameterRoomWName = CFSTR("Room width");
static const float kMinimumValue_RoomW = .0001;
static const float kMaximumValue_RoomW = 100.0;
static const float kDefaultValue_RoomW = 14.39;

static CFStringRef kParameterRoomDName = CFSTR("Room depth");
static const float kMinimumValue_RoomD = .0001;
static const float kMaximumValue_RoomD = 100.0;
static const float kDefaultValue_RoomD = 11.86;

static CFStringRef kParameterRoomHName = CFSTR("Room height");
static const float kMinimumValue_RoomH = .0001;
static const float kMaximumValue_RoomH = 100.0;
static const float kDefaultValue_RoomH = 10.0;

static CFStringRef kParameterSourceXName = CFSTR("Source X");
static const float kMinimumValue_SourceX = .0001;
static const float kMaximumValue_SourceX = 100.0;
static const float kDefaultValue_SourceX = 6.0;

static CFStringRef kParameterSourceYName = CFSTR("Source Y");
static const float kMinimumValue_SourceY = .0001;
static const float kMaximumValue_SourceY = 100.0;
static const float kDefaultValue_SourceY = 4.0;

static CFStringRef kParameterSourceZName = CFSTR("Source Z");
static const float kMinimumValue_SourceZ = .0001;
static const float kMaximumValue_SourceZ = 100.0;
static const float kDefaultValue_SourceZ = 3.0;

enum {
	kParam_RoomW,
	kParam_RoomD,
	kParam_RoomH,
	kParam_SourceX,
	kParam_SourceY,
	kParam_SourceZ,
	kNumberOfParameters
};

#pragma mark ____Babo
class BaboAU : public AUBase
{
public:
	BaboAU(AudioUnit component);
	~BaboAU();
	
	virtual ComponentResult		Initialize();
	
	virtual	OSStatus			GetParameterValueStrings(AudioUnitScope			inScope,
														 AudioUnitParameterID		inParameterID,
														 CFArrayRef *			outStrings);
    
	virtual	OSStatus			GetParameterInfo(AudioUnitScope			inScope,
												 AudioUnitParameterID	inParameterID,
												 AudioUnitParameterInfo	&outParameterInfo);
    
	virtual OSStatus			GetPropertyInfo(AudioUnitPropertyID		inID,
												AudioUnitScope			inScope,
												AudioUnitElement		inElement,
												UInt32 &			outDataSize,
												Boolean	&			outWritable );
	
	virtual OSStatus			GetProperty(AudioUnitPropertyID inID,
											AudioUnitScope 		inScope,
											AudioUnitElement 		inElement,
											void *			outData);
	
	UInt32						SupportedNumChannels(const AUChannelInfo ** outChannelInfo);

	
	virtual	ComponentResult 	Render(	AudioUnitRenderActionFlags &ioActionFlags,
									   const AudioTimeStamp &		inTimeStamp,
									   UInt32						nFrames);
	
	virtual bool				StreamFormatWritable(	AudioUnitScope					scope,
													 AudioUnitElement				element);
	 	
	virtual	bool				SupportsTail () { return true; }
	
	virtual Float64				GetTailTime();

	
	/*! @method Version */
	virtual OSStatus		Version() { return kBaboVersion; }
	
    
	
private: //state variables...
		BABO *BaboReverb;
		float sr, onedsr;
		
		// internal state
		float room_w, room_d, room_h;
		bool reverb_created;
};

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


#endif