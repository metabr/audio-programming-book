//
//  AUEditorView.m
//  AU
//
//  Created by Taemin Cho on 1/2/09.
//  Copyright 2009 NYU. All rights reserved.
//

#import "AUEditorView.h"

@implementation AUEditorView

+ (AUEditorView*) editorForAudioUnit:(AudioUnit)unit
{
	return [[[AUEditorView alloc] initWithAudioUnit:unit] autorelease];
}

+ (BOOL) pluginClassIsValid:(Class)pluginClass 
{
	if([pluginClass conformsToProtocol: @protocol(AUCocoaUIBase)]) {
		if([pluginClass instancesRespondToSelector: @selector(interfaceVersion)] &&
		   [pluginClass instancesRespondToSelector: @selector(uiViewForAudioUnit:withSize:)]) {
			return YES;
		}
	}
    return NO;
}

- (BOOL) hasCocoaView
{
	UInt32 dataSize   = 0;
	Boolean isWritable = 0;
	OSStatus err = AudioUnitGetPropertyInfo(mAudioUnit,
											kAudioUnitProperty_CocoaUI, kAudioUnitScope_Global,
											0, &dataSize, &isWritable);												// ... (1)
	
	return dataSize > 0 && err == noErr;
}

- (NSView *) getCocoaView
{
	NSView *theView = nil;
	UInt32 dataSize = 0;
	Boolean isWritable = 0;
	OSStatus err = AudioUnitGetPropertyInfo(mAudioUnit,
											kAudioUnitProperty_CocoaUI, kAudioUnitScope_Global, 
											0, &dataSize, &isWritable);												// ... (1)
	
	if(err != noErr)
		return theView;
	
	// If we have the property, then allocate storage for it
	AudioUnitCocoaViewInfo * cocoaViewInfo = (AudioUnitCocoaViewInfo*)malloc(dataSize);
	err = AudioUnitGetProperty(mAudioUnit, 
							   kAudioUnitProperty_CocoaUI, kAudioUnitScope_Global, 0, cocoaViewInfo, &dataSize);	// ... (2)
	
	// Extract CocoaView Information
	unsigned	numberOfClasses = (dataSize - sizeof(CFURLRef)) / sizeof(CFStringRef);								// ... (3)
	NSString	*viewClassName = (NSString*)(cocoaViewInfo->mCocoaAUViewClass[0]);									// ... (4)
	NSURL		*cocoaViewBundlePath = (NSURL *)cocoaViewInfo->mCocoaAUViewBundleLocation;
	NSBundle	*viewBundle = [NSBundle bundleWithPath:[cocoaViewBundlePath path]];
	Class		viewClass = [viewBundle classNamed:viewClassName];													// ... (5)
	
	if([AUEditorView pluginClassIsValid:viewClass]) {																// ... (6)
		id factory = [[[viewClass alloc] init] autorelease];
		theView = [factory uiViewForAudioUnit:mAudioUnit withSize: NSMakeSize(100, 100)];							// ... (7)
	}
	
	// Clean Up
	[cocoaViewBundlePath release];
	if(cocoaViewInfo) {
        int i;
        for(i = 0; i < numberOfClasses; i++)
            CFRelease(cocoaViewInfo->mCocoaAUViewClass[i]);
        
        free(cocoaViewInfo);
    }
	
	return theView;
}

- (void) createView
{
	NSView * view;
	
	if([self hasCocoaView])				// ... (1)
		view = [self getCocoaView];		// ... (2)
	else{
		view = [[AUGenericView alloc] initWithAudioUnit:mAudioUnit];	// ... (3)
		[(AUGenericView *)view setShowsExpertParameters:YES];			// ... (4)
		[view autorelease];												// ... (5)
	}	
	
	if(view) {
		NSPanel * panel = [[NSPanel alloc] initWithContentRect: [view frame] 
													 styleMask: NSTitledWindowMask | NSClosableWindowMask | NSUtilityWindowMask | NSTexturedBackgroundWindowMask
													   backing: NSBackingStoreBuffered defer:NO];
		[panel setContentView:view];	// ... (7)
		[panel setIsVisible:YES];
		[panel center];
		[self setWindow:panel];			// ... (8)
		[panel release];				// ... (9)
	}
}

- (id) initWithAudioUnit: (AudioUnit) unit
{
	if(self = [super init])
	{
		mAudioUnit = unit;
		[self createView];
	}
	
	return self;
}

- (void) dealloc
{
	[self setWindow:nil];

	[super dealloc];
}

@end
