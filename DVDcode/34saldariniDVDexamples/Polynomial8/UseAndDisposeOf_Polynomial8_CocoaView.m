/*
 *	File:		UseAndDisposeOf_Polynomial8_CocoaView.m
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

#import "UseAndDisposeOf_Polynomial8_CocoaView.h"


NSString* kBeginGestureNotification	= 
@"Polynomial_BeginGestureNotification";

NSString* kEndGestureNotification = 
@"Polynomial_EndGestureNotification";

#define kNumberOfCoefficients 9

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

static AudioUnitParameter parameter[kNumberOfParameters];


/*	Private methods (replace the template underscore versions in the
	interface file)
 */
@interface UseAndDisposeOf_Polynomial8_CocoaView ()

-(void) synchronizeUIWithParameterValues;
-(void) addListeners;
-(void) removeListeners;
-(void)	eventListenerHandler:	(void*)inObject 
		event:					(const AudioUnitEvent*)inEvent 
		value:					(Float32)inValue;

@end


//////////////////////////////////////////////////////

@implementation UseAndDisposeOf_Polynomial8_CocoaView

/*	Helper function: 
	Keeps all parameters handled by the GUI arranged in an array:
 */
void createAUParameterArray()
{
	for(int index = 0; index < kNumberOfParameters; index++)
	{
		parameter[index].mAudioUnit = NULL;
		parameter[index].mParameterID = index;
		parameter[index].mScope = kAudioUnitScope_Global;
		parameter[index].mElement = 0;
	}		
}

/*	Helper function:
	registers 
 */
void addEventsForParameters
(
	AUEventListenerRef	listener, 
	void*				refCon, 
	AudioUnitEvent*		inEvent
)
{	
	inEvent->mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
	verify_noerr
	(
		AUEventListenerAddEventType(listener, refCon, inEvent)
	);
	
	inEvent->mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	verify_noerr 
	(
		AUEventListenerAddEventType(listener, refCon, inEvent)
	);
	
	inEvent->mEventType = kAudioUnitEvent_ParameterValueChange;
	verify_noerr 
	(
		AUEventListenerAddEventType(listener, refCon, inEvent)
	);	
}

/*	Replaces the original parameter listener dispatcher so that
	this view can also handle begin and end gesture events.
	This is necessary for the view to support automation recording 
	through direct manipulation of the GUI by the user.
 
	Other than that, the function is almost the same as the parameter
	version.
 */
void eventListenerDispatcher
(
	void*					inRefCon, 
	void*					inObject, 
	const AudioUnitEvent*	inEvent, 
	UInt64					inHostTime, 
	Float32					inValue
)
{
	UseAndDisposeOf_Polynomial8_CocoaView* SELF = 
	(UseAndDisposeOf_Polynomial8_CocoaView*)inRefCon;
    
    [SELF eventListenerHandler:inObject event:inEvent value:inValue];
}


-(void) awakeFromNib
{		
	/*	Programatically modify certain attributes for the GUI objects
		as they are unarchived from the NIB file:
	 */
	
	NSFont* existingFont = [[waveShapingIndexField cell] font];
	
	[waveShapingIndexSlider setMinValue: 0.0];
	[waveShapingIndexSlider setMaxValue: 20.0];
	
	[outputLevelSlider setMinValue: -40.];
	[outputLevelSlider setMaxValue:	0.];
	
	[mixLevelSlider setMinValue: 0.];
	[mixLevelSlider setMaxValue: 100.];
	
	[coefficientFieldMatrix setCellSize:(NSSize){20.0,20.0}];
	[coefficientFieldMatrix setIntercellSpacing:(NSSize){6.6,0}];
	[coefficientFieldMatrix sizeToCells];
	
	[coefficientSliderMatrix setIntercellSpacing:(NSSize){6.6,0}]; 
	[coefficientSliderMatrix sizeToCells];
	for(int index = 0; index < 9; index++)
	{	
		[[coefficientSliderMatrix cellWithTag:index] setMinValue:-20.];
		
		[[coefficientSliderMatrix cellWithTag:index] setMaxValue: 20.];
		
		[[coefficientFieldMatrix cellWithTag:index] 
		 setFont: [[NSFontManager sharedFontManager] 
				   convertFont: existingFont 
				   toSize: 10]];
	}
	
	[[waveShapingIndexField cell] 
		setFont: [[NSFontManager sharedFontManager] 
		convertFont: existingFont 
		toSize: 11.5]];
	
	[[outputLevelField cell] 
		setFont: [[NSFontManager sharedFontManager] 
		convertFont: existingFont 
		toSize: 11.5]];
	
	[[mixLevelField cell] 
		setFont: [[NSFontManager sharedFontManager] 
		convertFont: existingFont 
		toSize: 11.5]];
	
	existingFont = nil;
}


-(void) dealloc 
{
    [self removeListeners];
    [[NSNotificationCenter defaultCenter] removeObserver: self];
	[super dealloc];
}


-(void) setAU: (AudioUnit)inAU 
{
	if (mAU) [self removeListeners];
	
	//	Bind this view to an instance of its corresponding audio unit:
	mAU = inAU;
	
	createAUParameterArray();	
	
	[self addListeners];
	[self synchronizeUIWithParameterValues];
	
	/*	Register this view to listen to begin and end gestures 
		for all GUI objects:
	 */
	[[NSNotificationCenter defaultCenter] 
		addObserver:	self 
		selector:		@selector(beginGesture:) 
		name:			kBeginGestureNotification 
		object:			waveShapingIndexSlider];
	
	[[NSNotificationCenter defaultCenter] 
		addObserver:	self 
		selector:		@selector(endGesture:) 
		name:			kEndGestureNotification 
		object:			waveShapingIndexSlider];
	
	[[NSNotificationCenter defaultCenter] 
		addObserver:	self 
		selector:		@selector(beginGesture:) 
		name:			kBeginGestureNotification 
		object:			outputLevelSlider];
	
	[[NSNotificationCenter defaultCenter] 
		addObserver:	self 
		selector:		@selector(endGesture:) 
		name:			kEndGestureNotification		
		object:			outputLevelSlider];
	
	[[NSNotificationCenter defaultCenter] 
		addObserver:	self 
		selector:		@selector(beginGesture:) 
		name:			kBeginGestureNotification 
		object:			mixLevelSlider];
	
	[[NSNotificationCenter defaultCenter] 
		addObserver:	self 
		selector:		@selector(endGesture:) 
		name:			kEndGestureNotification 
		object:			mixLevelSlider];
	
	[[NSNotificationCenter defaultCenter] 
		addObserver:	self 
		selector:		@selector(beginGesture:) 
		name:			kBeginGestureNotification 
		object:			coefficientSliderMatrix];
	
	[[NSNotificationCenter defaultCenter] 
		addObserver:	self 
		selector:		@selector(endGesture:) 
		name:			kEndGestureNotification 
		object:			coefficientSliderMatrix];
}


-(void) beginGesture:(NSNotification*)aNotification 
{
	
	/*	Find out what GUI object did the user just move 
		(waveshaping index, wet/dry mix, output attenuator or
		any coefficient slider) through the object's tag:
	 */
	id sender = [aNotification object];
	int parameterNumber = (sender == coefficientSliderMatrix) ? 
	[[sender selectedCell] tag] : [sender tag];
	
	/*	Look up the corresponding parameter in the parameter
		array (also through the GUI object's tag) and bind
		the appropiate parameter to this views' corresponding
		audio unit:
	 */
	parameter[parameterNumber].mAudioUnit = mAU;
	
	/*	Notify listeners that an audio unit parameter has
		just started changing (begin gesture - for automation purposes):
	 */
	AudioUnitEvent event;
	event.mArgument.mParameter = parameter[parameterNumber];
	event.mEventType = kAudioUnitEvent_BeginParameterChangeGesture;
	AUEventListenerNotify(eventListener, sender, &event);
}


-(void) endGesture: (NSNotification*)aNotification 
{	
	
	/*	Find out what GUI object did the user just move 
		(waveshaping index, wet/dry mix, output attenuator or
		any coefficient slider) through the object's tag:
	 */
	id sender = [aNotification object];
	int parameterNumber = (sender == coefficientSliderMatrix) ? 
	[[sender selectedCell] tag] : [sender tag];
	
	/*	Look up the corresponding parameter in the parameter
		array (also through the GUI object's tag) and bind
		the appropiate parameter to this views' corresponding
		audio unit:
	 */
	parameter[parameterNumber].mAudioUnit = mAU;
	
	/*	Notify listeners that an audio unit parameter has
		just finished changing (end gesture - for automation purposes):
	 */
	AudioUnitEvent event;
	event.mArgument.mParameter = parameter[parameterNumber];
	event.mEventType = kAudioUnitEvent_EndParameterChangeGesture;
	AUEventListenerNotify (eventListener, sender, &event);
}


-(IBAction) waveShapingIndexChanged: (id)sender 
{
	
	/*	Set the waveshaping index for the audio unit bound to this
		view to its new value:
	 */
	Float32 newValue = [sender floatValue];
	
	parameter[kWaveShapingIndex].mAudioUnit = mAU;
	
	NSAssert
	( 
		AUParameterSet
		(
			eventListener, 
			self, 
			&parameter[kWaveShapingIndex], 
			newValue, 
			0
		) == noErr,
	 
	 @"setting waveshaping index in audio unit"
	 
	);
	
	/*	Update the corresponding GUI control so that it also
		reflects the new value:
	 */
	if(sender == waveShapingIndexSlider)
		[waveShapingIndexField setFloatValue: newValue];
	else	
		[waveShapingIndexSlider setFloatValue: newValue];
	
}

-(IBAction) outputLevelChanged: (id)sender 
{
	/*	Set the output attenuator for the audio unit bound to this
		view to its new value:
	 */
    Float32 newValue = [sender floatValue];
	parameter[kOutputLevel].mAudioUnit = mAU;
	
	NSAssert
	( 
		AUParameterSet
		(
			eventListener, 
			self, 
			&parameter[kOutputLevel], 
			newValue, 
			0
		) == noErr,
	 
	 @"setting output level parameter in audio unit"
	 
	);
	
	/*	Update the corresponding GUI control so that it also
		reflects the new value:
	 */
	if(sender == outputLevelSlider)
		[outputLevelField setFloatValue: newValue];
	else	
		[outputLevelSlider setFloatValue: newValue];
}

-(IBAction) mixLevelChanged: (id)sender
{
	/*	Set the wet/dry mix level for the audio unit bound to this
		view to its new value:
	 */
	Float32 newValue = [sender floatValue];
	
	parameter[kMixLevel].mAudioUnit = mAU;
	
	NSAssert
	( 
		AUParameterSet
		(
			eventListener, 
			self, 
			&parameter[kMixLevel], 
			newValue, 
			0
		) == noErr,
	 
	 @"setting waveshaping index in audio unit"
	 
	);
	
	/*	Update the corresponding GUI control so that it also
		reflects the new value:
	 */
	if(sender == mixLevelSlider)
		[mixLevelField setFloatValue: newValue];
	else	
		[mixLevelSlider setFloatValue: newValue];
}

-(IBAction) CoefficientsChanged: (id)sender
{
	/*	Set a coefficient for the audio unit bound to this
		view to its new value:
	 */
	int coefficientNumber	= [[sender selectedCell] tag];
	Float32 newValue		= [[sender selectedCell] floatValue];
	
	parameter[coefficientNumber].mAudioUnit = mAU;
	
	NSAssert
	( 
		AUParameterSet
		(
			eventListener, 
			self, 
			&parameter[coefficientNumber], 
			newValue, 
			0
		) == noErr,
	 
	 @"setting coefficient parameter in audio unit"
	 
	);	
	
	coefficientArray[coefficientNumber] = newValue;
	
	//	Tell the graph subview to update the transfer function graph:
	[graphView 
		updateTransferFunctionGraphWithData:coefficientArray 
		ofSize:kNumberOfCoefficients];	
	
	/*	Update the corresponding GUI control so that it also
		reflects the new value:
	 */
	if(sender == coefficientSliderMatrix)
		[[coefficientFieldMatrix cellWithTag:coefficientNumber] 
			setFloatValue:newValue];
	else
		[[coefficientSliderMatrix cellWithTag:coefficientNumber] 
			setFloatValue:newValue];
}

#pragma mark ____ PRIVATE FUNCTIONS ____
-(void) addListeners 
{	
	if (!mAU) return;
	
	verify_noerr
	(
		AUEventListenerCreate
		(
			eventListenerDispatcher, 
			self,
			CFRunLoopGetCurrent(), 
			kCFRunLoopDefaultMode, 
			0.05, 
			0.05, 
			&eventListener
		)
	 );
	
	AudioUnitEvent auEvent;
	
	/*	Use our helper function to register this view to listen for
		begin and end gesture events for all objects in the GUI:
	 */
	for(int index = 0; index <kNumberOfParameters; index++)
	{
		parameter[index].mAudioUnit = mAU;
		auEvent.mArgument.mParameter = parameter[index];		
		addEventsForParameters(eventListener, self, &auEvent);
	}		
}

-(void) removeListeners 
{
	if (eventListener) verify_noerr (AUListenerDispose(eventListener));
	
	eventListener = NULL;
	mAU = NULL;	
}

-(void) synchronizeUIWithParameterValues 
{
	
	//	Executed only once when the view initializes:
	
	/*	Get all parameter values from the audio unit bound to this 
		view and update all GUI objects to reflect the audio unit's
		current values:
	 */
	Float32 value;
	int index;
	
    for (index = 0; index < kNumberOfCoefficients; index++) 
	{
		NSAssert 
		(
			AudioUnitGetParameter
			(
				mAU, 
				index, 
				kAudioUnitScope_Global, 
				0, 
				&value
			) == noErr,
		 
		 @"[UseAndDisposeOf_Polynomial8_CocoaView synchronizeUIWithParameterValues] coefficients"
		 
		 );
		
		coefficientArray[index] = value;
		
		[[coefficientFieldMatrix cellWithTag:index] 
		 setFloatValue:value];
		
		[[coefficientSliderMatrix cellWithTag:index] 
		 setFloatValue:value];
	}
	
	NSAssert 
	(
		AudioUnitGetParameter
		(
			mAU, 
			kWaveShapingIndex, 
			kAudioUnitScope_Global, 
			0, 
			&value
		) == noErr,
	 
	 @"[UseAndDisposeOf_Polynomial8_CocoaView synchronizeUIWithParameterValues] waveshaping index"
	 
	 );
	
	[waveShapingIndexSlider setFloatValue:value];
	[waveShapingIndexField setFloatValue:value];
	
	NSAssert 
	(
		AudioUnitGetParameter
		(
			mAU, 
			kOutputLevel, 
			kAudioUnitScope_Global, 
			0, 
			&value
		) == noErr,
	 
	 @"[UseAndDisposeOf_Polynomial8_CocoaView synchronizeUIWithParameterValues] output level"
	 
	);
	
	[outputLevelSlider setFloatValue:value];
	[outputLevelField setFloatValue:value];
	
	NSAssert 
	(
		AudioUnitGetParameter
		(
			mAU, 
			kMixLevel, 
			kAudioUnitScope_Global, 
			0, 
			&value
		) == noErr,
	 
		@"[UseAndDisposeOf_Polynomial8_CocoaView synchronizeUIWithParameterValues] mix level"
	 
	 );
	
	[mixLevelSlider setFloatValue:value];
	[mixLevelField setFloatValue:value];
	
	[graphView 
		updateTransferFunctionGraphWithData:coefficientArray 
		ofSize:kNumberOfCoefficients];	
}


-(void) eventListenerHandler:	(void*)inObject 
		event:					(const AudioUnitEvent*)inEvent 
		value:					(Float32)inValue 
{
	
	/*	This method's task is similar to that of the IBAction and 
		begin/end gesture methods combined, only that it applies 
		to the case where the view needs to update GUI objects
		in response to event notifications.
	 
		This is the case, for instance, when a host is reproducing
		previously recorded automation data.
	 */
	int parameterID = inEvent->mArgument.mParameter.mParameterID;
	
	switch(inEvent->mEventType) 
	{
		/*	For streams of parameter value changes,
			we just update the appropiate GUI objects to reflect
			the new values:
		 */
		case kAudioUnitEvent_ParameterValueChange:
		{
			switch(parameterID) 
			{
				case 0:
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				{
					[[coefficientFieldMatrix cellWithTag:parameterID] 
						setFloatValue: inValue];
				
					[[coefficientSliderMatrix cellWithTag:parameterID] 
						setFloatValue: inValue];
				
					coefficientArray[parameterID] = inValue;
				
					[graphView
						updateTransferFunctionGraphWithData:coefficientArray 
						ofSize:kNumberOfCoefficients];
					break;
				}
				
				case kWaveShapingIndex:
				{
					[waveShapingIndexSlider setFloatValue:inValue];
					[waveShapingIndexField setFloatValue:inValue];
					break;
				}
				
				case kOutputLevel:
				{
					[outputLevelSlider setFloatValue:inValue];
					[outputLevelField setFloatValue:inValue];
					break;	
				}
				
				case kMixLevel:
				{
					[mixLevelSlider setFloatValue:inValue];
					[mixLevelField setFloatValue:inValue];
					break;
				}
			}
			break;
		}
		
		/*	For begin and end gesture notifications,
			we don't really need to do anything:
			sliders and fields will continuously change
			anyways, based on the stream of parameter changes.
		 */
		case kAudioUnitEvent_BeginParameterChangeGesture:
			// Start gesture
			break;
			
		case kAudioUnitEvent_EndParameterChangeGesture:				
			// End gesture
			break;
	}
}

@end

