//
//  TrackingView.h
//  iSight_Controller
//
//  Created by Taemin Cho on 1/29/08.
//  Copyright 2008 __Taemin Cho__. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#define SEARCH_STEP 4

@interface TrackingView : NSView {

	CIImage		*image;
	
	NSPoint		downPoint, currentPoint; // for mouse event
	NSPoint		startXY;	// searching start point
	
	BOOL		LUTFlag, trackingFlag, mirrorFlag;
	
	NSMutableSet *LookUpTable; //color Look Up Table
}

- (void)setAnImage:(CIImage *)newImage;
- (NSRect)currentRect;
- (id)colorDepth:(NSUInteger[])p;
- (void)setLUTFlag:(BOOL)flag;
- (void)setLUTFrom:(NSRect)selRect Of:(NSBitmapImageRep*)bitmapRep;
- (void)resetLUT;
- (NSRect)trackingAt:(NSBitmapImageRep*)bitmapRep;
- (void)setTrackingFlag:(BOOL)flag;
- (void)setMirrorFlag:(BOOL)flag;
- (NSPoint)getCenterXY;
@end
