/*
 *	File:		UseAndDisposeOf_Polynomial_GraphView.h
 *	
 *	Version:	1.0
 * 
 *	Created:	9/28/08 by Federico Saldarini
 *	
 *	Copyright:  Copyright © 2009 UseAndDisposeOf_, All Rights Reserved
 * 
 *	Contact:	fsaldarini@useanddisposeof.com
 */

#import <Cocoa/Cocoa.h>


@interface UseAndDisposeOf_Polynomial_GraphView : NSView 
{
	CGLayerRef			background;
	CGMutablePathRef	transferFunctionPath;
}

-(void)	updateTransferFunctionGraphWithData:(Float32*)theData 
		ofSize:								(int)size;

@end