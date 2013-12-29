/*
 *	File:		NSMatrixCustom.m
 *	
 *	Version:	1.0
 * 
 *	Created:	9/28/08 by Federico Saldarini
 *	
 *	Copyright:  Copyright © 2009 UseAndDisposeOf_, All Rights Reserved
 * 
 *	Contact:	fsaldarini@useanddisposeof.com
 */

#import "NSMatrixCustom.h"

extern NSString *kBeginGestureNotification;
extern NSString *kEndGestureNotification;

@implementation NSMatrixCustom

-(void) mouseDown:(NSEvent *) theEvent 
{
	[[NSNotificationCenter defaultCenter] 
		postNotificationName: kBeginGestureNotification 
		object: self];
	
	[super mouseDown:theEvent];
	
	[[NSNotificationCenter defaultCenter] 
		postNotificationName: kEndGestureNotification 
		object: self];
}

@end





		
	
	