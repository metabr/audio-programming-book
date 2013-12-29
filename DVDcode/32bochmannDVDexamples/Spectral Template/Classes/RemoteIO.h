//
//  RemoteIO.h
//  Spectral Template
//
//  Created by Johannes Bochmann on 15.04.09.
//  Copyright 2009 JoboMusic GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InMemoryAudioFile.h"
#import "globals.h"

#include "FFTBufferMatrix.h"



@interface RemoteIO : NSObject {
	
	
	InMemoryAudioFile *inMemoryAudiofile;
	
	float *channel1InputSampleBuffer;
	float *channel1OutputSampleBuffer;
	float *channel1MagnitudePhasesBuffer;
	float *channel1MagnitudePhasesFrame;

	
	float *fftWindow;
	
	FFTBufferMatrix *bufferMatrix;

	int currentMatrixFrame;	
	int loadPercent;
	
}

	
@property (nonatomic, retain) InMemoryAudioFile *inMemoryAudiofile;
@property (readonly, nonatomic) FFTBufferMatrix *bufferMatrix;
@property (readonly, nonatomic) int loadPercent;


-(id)initWithFile:(NSString *)file;

-(void)initializeAudio;
-(OSStatus)start;
-(OSStatus)stop;
-(void)checkStatus:(OSStatus) status;

-(void)analyzeAudioFile:(NSString *)file;

-(float)convertInt16ToNormalizedFloat:(SInt16) input;
-(SInt16)convertNormalizedFloatToInt16:(float) input;

-(void)cleanUp;


@end
