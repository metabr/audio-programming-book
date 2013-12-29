//
//  SineEvent.m
//  TestHidOsc
//
//  Created by Andrew Beck on 1/18/08.
//

#import "SineEvent.h"

@implementation SineEvent

- (id) initWithBuf:(float[]) setBuf
{
	[super init];
	
	sNumChannels = 1;
	sWhichFormat = 32;
	j = 0;
	amp = 0.01;
	freq = 440.;
	[self setAtk:(int) (44100*0.01)];
	[self setDelay:0];
	[self setDuration:44100];
	theFormatID = kAudioFormatLinearPCM;
// these are the same regardless of format
	theFramesPerPacket = 1; // this shouldn't change
	bufSize = 1024;
	int i;
	for (i=0; i<bufSize+1; i++)
		buf[i] = setBuf[i];
	
	return self;
}

- (void) setFreq:(double) setFreq
{
	if (setFreq < 22050 && setFreq > 0)
		freq = setFreq;// * ((random()*0.001)+0.9995);
}

- (double) freq
{
	return freq * exp((1-[self age])*finalFreqExp);
}

- (void) setDelay:(int) setDelay
{
	delay = setDelay;
}

- (void) setFinalFreq:(double) finalFreq
{
	float finalRatio = finalFreq / freq;
	[self setFinalFreqRatio:finalRatio];
}
- (void) setFinalFreqRatio:(double) finalFreqRatio
{
	finalFreqExp = log(finalFreqRatio);
}

- (void) setAmp:(double) setAmp
{
	amp = setAmp;
}

- (float) amp
{
	ampCount++;
	if (ampCount>31 || resetAmp)
	{
		if (ampCount>31 && (atk < fullAtk))
			atk+=32;
		if (atk < fullAtk)
		{
			curAmp =  amp * [self age] * log10((atk / fullAtk) * 9 + 1);
		}
		else
			curAmp =  amp * [self age];
		
		if (ampCount>31)
			ampCount=0;
	}
	
	return curAmp;
}

- (void) setAtk:(double) setAtk
{
	fullAtk = setAtk;
	atk = 1;
	resetAmp = YES;
}

- (void) setDuration:(double) setDur
{
	fullDur = setDur;
	duration = setDur;
	age = 1;
}

- (double) duration
{
	return duration;
}

- (void) decDurBy:(float) sub
{
	duration = duration - sub;
	age = (duration / fullDur);
}

- (double) age
{
	return age;
}

- (BOOL) isDead
{
	return ([self duration] <= 0);
}

- (void) render:(unsigned long)inFrames toBuf:(void*)inBuffer atSR:(double)inSampleRate inFormat:(int)inOutputFormat
{
	unsigned long nSampleFrames = inFrames;
	float *sourceP = inBuffer;
	double incr = [self freq] / inSampleRate;
    
	while (nSampleFrames-- > 0) {
		if (delay < 1)
		{
			float inputSample = *sourceP;
			float i = (j*bufSize);
			float interp = i - ((int) i);
			float nextFloat = inputSample + ( 
					((buf[(int) i] * interp) + (buf[(int) i+1] * (1-interp))) * [self amp]);

	//		switch (inOutputFormat) {
	//			case 32:
	//				((Float32*) inBuffer)[frame] = nextFloat;
	//				break;
	//			case 16:
	//				((SInt16*) inBuffer)[frame] = (SInt16)(nextFloat * 32768. + 0.5);
	//				break;
	//				
	//			case 24:
	//				((UInt32*) inBuffer)[frame] = (UInt32)(nextFloat * 8388608. + 0.5);
	//				break;
	//		}
			*sourceP = nextFloat;

			j = j + incr;
			if (j > 1.0)
				j = j - 1.0;
		} else {
			delay--;
		}
	
		sourceP = sourceP + 1;
	}
	
	if (delay < 1)
		[self decDurBy:inFrames];
}

@end
