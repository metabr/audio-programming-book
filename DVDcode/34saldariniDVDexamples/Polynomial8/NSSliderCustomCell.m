/*
 *	File:		NSSliderCustomCell.m
 *	
 *	Version:	1.0
 * 
 *	Created:	9/28/08 by Federico Saldarini
 *	
 *	Copyright:  Copyright © 2009 UseAndDisposeOf_, All Rights Reserved
 * 
 *	Contact:	fsaldarini@useanddisposeof.com
 */

#import "NSSliderCustomCell.h"


@implementation NSSliderCustomCell

-(void) awakeFromNib 
{	
	NSBundle	*bundle;
	NSString	*imageName;
	
	bundle = [NSBundle bundleForClass:[self class]];
	
	imageName	= [bundle pathForImageResource: @"PolynomialKnobOn"];
	knobOn		= [[NSImage alloc] initWithContentsOfFile: imageName];
	
	NSSize size = {21.0,21.0};
	
	[knobOn		setSize:size];
}

-(void) dealloc
{
	[knobOn	release];
	[super dealloc];
}

-(BOOL) _usesCustomTrackImage {return YES;}

-(void) drawKnob:(NSRect)knobRect 
{		
	[[self controlView] lockFocus];
	
	[knobOn 
		compositeToPoint: 
			NSMakePoint
			(
				knobRect.origin.x,  
				knobRect.origin.y + knobRect.size.height
			)
		operation: NSCompositeSourceOver];
	
	[[self controlView] unlockFocus];	
}

@end
