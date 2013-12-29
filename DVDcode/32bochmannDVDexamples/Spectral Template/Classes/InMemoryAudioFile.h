//
//  InMemoryAudioFile.h
//  HelloWorld
//
//  Created by Aran Mulholland on 22/02/09.
//  Copyright 2009 Aran Mulholland. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioFile.h>
#include <sys/time.h>


@interface InMemoryAudioFile : NSObject {
	AudioStreamBasicDescription		mDataFormat;                    
    AudioFileID						mAudioFile;                     
    UInt32							bufferByteSize;                 
    SInt64							mCurrentPacket;                 
    UInt32							mNumPacketsToRead;              
    AudioStreamPacketDescription	*mPacketDescs;                  
	SInt64							packetCount;
	SInt16							*audioData;
	SInt64							sampleIndex;

}

//returns the number of Packets (=frames) of the audio File
@property (readonly, nonatomic) SInt64 packetCount;

//opens a wav file
-(OSStatus)open:(NSString *)filePath;
//gets the infor about a wav file, stores it locally
-(OSStatus)getFileInfo;

//gets the next packet from the buffer, returns -1 if we have reached the end of the buffer
-(SInt16)getNextSample;

//gets the current index (where we are up to in the buffer)
-(SInt64)getIndex;

//reset the index to the start of the file
-(void)reset;

@end
