//
//  TrackingView.m
//  iSight_Controller
//
//  Created by Taemin Cho on 1/29/08.
//  Copyright 2008 __Taemin Cho__. All rights reserved.
//

#import "TrackingView.h"


@implementation TrackingView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        image = nil;
		LookUpTable = [[NSMutableSet alloc] initWithCapacity:0]; // create LUT
		LUTFlag = NO;
		trackingFlag = NO;
		mirrorFlag = YES;
    }
    return self;
}

- (void)drawRect:(NSRect)rect {
	
	NSRect selRect;
	
	if (image){	
		
		if(mirrorFlag){
			NSAffineTransform* xform = [NSAffineTransform transform]; // create a transformation matrix
			[xform translateXBy:rect.size.width yBy:0.0]; // Applies the specified translation factors
			[xform scaleXBy:-1.0 yBy:1.0]; // flip the x axis
			[xform concat]; // store the transformation matrix in the current graphics context
		}
		
		CIImage *tempImage = [[image copy] autorelease];
		NSBitmapImageRep *bitmapRep = [[[NSBitmapImageRep alloc] initWithCIImage:tempImage] autorelease];
		NSImage *nsImage = [[[NSImage alloc] initWithSize:[bitmapRep size]] autorelease];
		
		[nsImage drawRepresentation:bitmapRep inRect: rect];
		
		if(trackingFlag){
			selRect = [self trackingAt:bitmapRep];	// ... (1)
			
			[[NSColor yellowColor] set];	// ... (2)
			[NSBezierPath strokeLineFromPoint:NSMakePoint(NSMidX(selRect),0) 
									  toPoint:NSMakePoint(NSMidX(selRect),479)]; // ... (3)
			[NSBezierPath strokeLineFromPoint:NSMakePoint(0,NSMidY(selRect))  
									  toPoint:NSMakePoint(639,NSMidY(selRect))]; // ... (3)
			[[NSColor redColor] set];		// ... (4)
			
		}else{
			selRect = [self currentRect];
			
			if(mirrorFlag)
				selRect.origin.x = rect.size.width - selRect.origin.x - selRect.size.width;
			
			if (LUTFlag==YES){
				[self setLUTFrom:selRect Of:bitmapRep];
				startXY = NSMakePoint(NSMidX(selRect), NSMidY(selRect));
			}
			[[NSColor greenColor] set];
		}
		
		[NSBezierPath strokeRect:selRect];
	}
}

- (void)setAnImage:(CIImage *)newImage
{
	[newImage retain];
	[image release];
	image = newImage;
	[self setNeedsDisplay:YES];
}

- (void)mouseDown:(NSEvent *)event
{
	downPoint = [self convertPointFromBase:[event locationInWindow]];
	currentPoint = downPoint;
	[self setNeedsDisplay:YES];
}

- (void)mouseDragged:(NSEvent *)event
{
	currentPoint = [self convertPointFromBase:[event locationInWindow]];
	[self setNeedsDisplay:YES];
}

- (void)mouseUp:(NSEvent *)event
{
	currentPoint = [self convertPointFromBase:[event locationInWindow]];
	[self setNeedsDisplay:YES];
}

- (NSRect)currentRect
{
	CGFloat minX = MIN(downPoint.x, currentPoint.x);
	CGFloat maxX = MAX(downPoint.x, currentPoint.x);
	CGFloat minY = MIN(downPoint.y, currentPoint.y);
	CGFloat maxY = MAX(downPoint.y, currentPoint.y);
	
	if(minX < 0.0) minX = 0.0;
	if(maxX > 639.0) maxX = 639.0;
	if(minY < 0.0) minY = 0.0;
	if(maxY > 479.0) maxY = 479.0;
	
	return NSMakeRect(minX, minY, maxX-minX, maxY-minY);
}

// change RGBA to RGB555
- (id)colorDepth:(NSUInteger[])p
{
	UInt16 r,g,b,a;
	r = p[0] >> 3; g = p[1] >> 3; b = p[2] >> 3;
	a = r << 10 | g << 5 | b;
	return [NSNumber numberWithUnsignedShort:a];
}

- (void)setLUTFrom:(NSRect)selRect Of:(NSBitmapImageRep*)bitmapRep
{	
	int x, y;
	NSUInteger p[4];
	
	for (x = (int)NSMinX(selRect) ; x <= (int)NSMaxX(selRect) ; x++)
		for(y = (int)NSMinY(selRect) ; y <= (int)NSMaxY(selRect) ; y++){
			[bitmapRep getPixel:p atX:x y:(480-y-1)]; // get pixel data from bitmapRep
			[LookUpTable addObject:[self colorDepth:p]]; // store the color data in LUT
		}

	LUTFlag = NO;
}

- (void)setLUTFlag:(BOOL)flag
{
	LUTFlag = flag;
	[self setNeedsDisplay:YES];
}

- (void)resetLUT
{
	[LookUpTable removeAllObjects];
}

- (NSRect)trackingAt:(NSBitmapImageRep*)bitmapRep
{		
	CGFloat minX, minY, maxX, maxY;
	NSRect rect = NSMakeRect(0.0, 0.0, 0.0, 0.0);
	
	int flag= NO;
	int step = SEARCH_STEP;			// Searching Step ........... (1)
	int x, y;
	int xcount = 0, ycount = 0;
	int i, j;
	NSUInteger p[4];
	
	x = startXY.x;  y = startXY.y; // set the start point
	
	for(i = 1; i < 1280/SEARCH_STEP; i++) // main Loop.
	{
		step *= -1;			// change the searching direction  ...... (2)
		
		for( xcount ; xcount < i ; xcount++ ) // x axis searching loop
		{
			if ( y > 479 | y < 0 )		// ..................... (3)
			{
				if (y > 479){			// free count the overflows
					ycount = (y - 479)/SEARCH_STEP;
					y = 479; 
				}else{
					ycount = abs(y)/SEARCH_STEP;
					y = 0;
				}	
				
				x += step*(i-xcount); // skip x;				
				break;
			}
			
			[bitmapRep getPixel:p atX:x y:(480-y-1)];				// ... (4)
			if([LookUpTable containsObject:[self colorDepth:p]]){	// ... (5) 
				flag = YES;
				break;
			}
			
			x += step;				// ......................... (6)								
			
			if ( x > 639 | x < 0 )  // ......................... (7)
			{
				x += step*(i-xcount-1); // skip remain x
				break;
			} else
				ycount = 0;
		}
		
		if (flag == YES) break; // if find then break the main loop ... (8)
		
		for( ycount ; ycount < i ; ycount++) // x axis searching loop
		{
			if ( x > 639 | x < 0 )
			{
				if (x > 639){
					xcount = (x - 639)/SEARCH_STEP;
					x = 639;
				}else{
					xcount = abs(x)/SEARCH_STEP;
					x = 0;
				}
				
				y += step*(i-ycount); // skip y
				break;
			}
			[bitmapRep getPixel:p atX:x y:(480-y-1)];
			if([LookUpTable containsObject:[self colorDepth:p]]){
				flag = YES;
				break;
			}
			
			y += step;
			
			if ( y > 479 | y < 0 )
			{
				y += step*(i-ycount-1); //skip remain y
				break;
			}else
				xcount = 0;
		}
		
		if (flag == YES) break; // if find then break the main loop
	}
	
	if(flag)		// if matched pixel is found ...... (9)
	{
		minX = x;	maxX = x;
		minY = y;	maxY = y;
		
		for (i = x; i > 0; i--)
		{
			for(j = y; j > 0 ;j--){
				[bitmapRep getPixel:p atX:i y:(480-j-1)];
				if(![LookUpTable containsObject:[self colorDepth:p]]) // if NOT, be careful the NOT operator... (10)
					break;						
				if(minY > j)
					minY = j;
				if(minX > i)
					minX = i;
			}
			if(j==y)
				break;
		}
		
		for (i = x; i < 639 ;i++)
		{
			for(j = y; j < 479 ; j++){
				[bitmapRep getPixel:p atX:i y:(480-j-1)];
				if(![LookUpTable containsObject:[self colorDepth:p]]) // if NOT, be careful the NOT operator... (10)
					break;
				if(maxY < j)
					maxY = j;
				if(maxX < i)
					maxX = i;
			}
			
			if(j==y)
				break;
		}
		
		rect = NSMakeRect(minX, minY, maxX-minX, maxY-minY); // found object ... (11)
		
		startXY=NSMakePoint(floor(NSMidX(rect)),floor(NSMidY(rect))); // center of the object ... (12)
		
		[[NSNotificationCenter defaultCenter] 
		 postNotificationName:@"TrackingNotification" object:self];
 		
	} else
		startXY = NSMakePoint(320.0,240.0);  // if not found. ... (13)
	
	return rect;
}

- (void)setTrackingFlag:(BOOL)flag
{
	trackingFlag = flag;
	[self setNeedsDisplay:YES];
}

- (void)setMirrorFlag:(BOOL)flag
{
	mirrorFlag = flag;
	[self setNeedsDisplay:YES];
}

- (NSPoint)getCenterXY{
	NSPoint centerXY = startXY;
	if(mirrorFlag)
		centerXY.x = 639 - centerXY.x;
	return centerXY;
}

- (void)dealloc
{
	[image release];
	[LookUpTable release];
	[super dealloc];
}
@end
