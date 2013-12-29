/*
 *	File:		UseAndDisposeOf_Polynomial_GraphView.m
 *	
 *	Version:	1.0
 * 
 *	Created:	9/28/08 by Federico Saldarini
 *	
 *	Copyright:  Copyright © 2009 UseAndDisposeOf_, All Rights Reserved
 * 
 *	Contact:	fsaldarini@useanddisposeof.com
 */


#import "UseAndDisposeOf_Polynomial_GraphView.h"

//	Private method to cach the background. Avoids unnecessary redrawing.
@interface UseAndDisposeOf_Polynomial_GraphView ()

-(CGLayerRef)	cacheBackground:	(CGLayerRef)layer 
				withContext:		(CGContextRef)context 
				andFrame:			(CGRect)frame;

@end

@implementation UseAndDisposeOf_Polynomial_GraphView

-(id)initWithFrame:(NSRect)frame 
{
	//	Allocate and initialize an instance of the graph subview:
	self = [super initWithFrame:frame];
    if (!self)
		return nil;
	
	//	Get this subview's drawing context (created by Quartz)
	CGContextRef context = 
	[[NSGraphicsContext currentContext] graphicsPort];
	
	//	Create a Quartz frame to do low-level drawing onto:
	CGRect QuartzFrame = 
	CGRectMake
	(
		frame.origin.x, 
		frame.origin.y, 
		frame.size.width, 
		frame.size.height
	);
	
	NSAssert
	(	
		//	Cach the subview's background grid:
		(background = [self 
							cacheBackground:background 
							withContext:context 
							andFrame:QuartzFrame]
		),
		
		@"Error generating context\n"
	);	
	
	context	= NULL;
	
	//	Return the initialized subview:
	return self;
}

-(void)dealloc 
{
	CFRelease(background);
	CFRelease(transferFunctionPath);
	
	[super dealloc];
}


-(void)drawRect:(NSRect)rect 
{	
	//	Get the current drawing context:
	CGContextRef context = 
	[
		[NSGraphicsContext currentContext] graphicsPort
	];
	
	CGRect r = 
	CGRectMake
	(
		rect.origin.x, 
		rect.origin.y, 
		rect.size.width, 
		rect.size.height
	);
	
	//	Paste the cached background onto the context:
	CGContextDrawLayerAtPoint(context, CGPointMake(0,0), background);
	
	/*	Modify the translation/scaling matrices so that the center
		of the subview maps to x,y coordinates 0,0: 
	 */
	CGContextTranslateCTM(context, r.size.width / 2, r.size.height / 2);
	CGContextScaleCTM(context, r.size.width / 2, r.size.height / 2);
	
	/*	Set a suitable color and line width to draw the 
		transfer function:
	 */
	CGContextSetRGBStrokeColor(context, 0.696, 0.927, 0.990, 1);
	CGContextSetLineWidth(context, 0.008);
	
	//	Paste the pre-computed path onto the context and trace it:
	CGContextAddPath(context, transferFunctionPath);
	CGContextStrokePath(context);
}

-(void) updateTransferFunctionGraphWithData:(Float32*)theData 
		ofSize:								(int)size
{
	//	The transfer function plot is always made up of 100 points:
	CGPoint			pointArray[100];
	int index		= 99;
	float step		=  2 / 50.;
	float domain	= 1.;
	float range;
	
	int	lastCoefficient			= size - 1;
	int secondToLastCoefficient	= lastCoefficient - 1;
	
	/*	Compute the transfer function plot using an adaptation
		of the DSP algorithm in Polynomial8.cpp.
		
		We are basically waveshaping 100 samples and keeping
		both the original and processed samples. The result is
		an array of 100 points with x,y coordinates interpreted
		as domain (dry) and range (wet) values, respectively:
	 */
	do	
	{	
		range = theData[lastCoefficient];
		
		for (int i = secondToLastCoefficient; i >= 0; --i) 
		{
			range *= domain;
			range += theData[i];
		}
		
		pointArray[index].x = domain;
		pointArray[index].y = range;
		domain -= step;
	} while (index--);

	//	Release any refference to a previous path, if any:
	if(transferFunctionPath) CFRelease(transferFunctionPath);
	
	/*	Create a new blank path and add to it segments according to 
		the plot data previously computed:
	 */
	transferFunctionPath = CGPathCreateMutable();
	CGPathAddLines(transferFunctionPath, NULL, pointArray, 100);
	
	/*	Once the path is computed, we signal that the subview should
		be redrawn, thus updating the graph in the GUI:
	 */
	[self setNeedsDisplay:YES];
}

-(CGLayerRef)	cacheBackground:(CGLayerRef)layer 
				withContext:	(CGContextRef)context 
				andFrame:		(CGRect)frame
{
	//	Initialize a valid layer:
	if(!(layer = CGLayerCreateWithContext(context, frame.size, NULL)))
		return nil;
	
	CGContextRef layerContext = CGLayerGetContext(layer);
	
	if(!layerContext)
	{
		CFRelease(layerContext);
		return nil;
	}
	
	//	Calculate the grid subdivision based on the subview's size:
	float step = frame.size.width/10;
	
	//	Set an appropiate color for the grid lines:
	CGContextSetRGBStrokeColor(layerContext, 0.596, 0.827, 0.890, 0.3);
	
	/*	Issue a series of CG calls to draw the horizontal and vertical
		lines of the grid.
		This is very similar to drawing with OpenGL calls...
	 */
	CGContextBeginPath(layerContext);
	
	for(float w = 0; w < frame.size.width; w += step)
	{
		CGContextMoveToPoint(layerContext, w, 0);
		CGContextAddLineToPoint(layerContext,w, frame.size.width);
	}
	
	for(float h = 0; h < frame.size.width; h += step)
	{
		CGContextMoveToPoint(layerContext, 0, h);
		CGContextAddLineToPoint(layerContext, frame.size.width, h);
	}
	
	CGContextStrokePath(layerContext);
	
	
	step /= 2;
	
	CGContextSetRGBStrokeColor(layerContext, 0.596, 0.527, 0.590, 0.1);
	for(float w = 0; w < frame.size.width; w += step)
	{
		CGContextMoveToPoint(layerContext, w, 0);
		CGContextAddLineToPoint(layerContext,w, frame.size.width);
	}
	
	for(float h = 0; h < frame.size.width; h += step)
	{
		CGContextMoveToPoint(layerContext, 0, h);
		CGContextAddLineToPoint(layerContext, frame.size.width, h);
	}
	
	CGContextStrokePath(layerContext);
	
	CGContextSetRGBStrokeColor(layerContext, 0.596, 0.827, 1., 0.4);
	
	CGContextMoveToPoint(layerContext, frame.size.width / 2, 0);
	
	CGContextAddLineToPoint
	(
		layerContext,
		frame.size.width / 2, 
		frame.size.height
	);
	
	CGContextStrokePath(layerContext);
	
	CGContextMoveToPoint(layerContext, 0, frame.size.height / 2);
	
	CGContextAddLineToPoint
	(
		layerContext,
		frame.size.width, 
		frame.size.height/2
	);
	
	CGContextStrokePath(layerContext);
	
	/*	Once we are done drawing into the layer, the layer stays
		baked in a way similar to an OpenGL list. This means we can
		now reuse this layer as many times as we want, and there's no
		need to re-issue any of the previous drawing calls.
	 */
	
	//	Return the baked layer:
	return layer;
}	

@end
