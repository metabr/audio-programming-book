/*
 *  OscilUnit.h
 *  OscilUnit
 *
 *  Created by Mehul Trivedi on 9/12/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "OscilUnitVersion.h"
#include "AUInstrumentBase.h"


// global variables for wave tables
const int kWaveArraySize = 4096;
const int PHMASK = (int) 0x0FFF;

const int kHarmonics = 25;

float mSine [kWaveArraySize];
float mSquare [kWaveArraySize];
float mSawtooth [kWaveArraySize];
float mTriangle [kWaveArraySize];

// 
static const UInt32 kNumNotes = 12;
static const UInt32 kMaxActiveNotes = 8;

// parameter info
static CFStringRef kParamName_Wavetable = CFSTR ("Wavetable");
static const int kSineWave_Wavetable     = 1;
static const int kSquareWave_Wavetable   = 2;
static const int kSawtoothWave_Wavetable = 3;
static const int kTriangleWave_Wavetable = 4;

static const int kDefaultValue_Wavetable = kSineWave_Wavetable;

// menu item names for the wavetable parameter
static CFStringRef kMenuItem_Wavetable_Sine       = CFSTR ("Sine");
static CFStringRef kMenuItem_Wavetable_Square     = CFSTR ("Square");
static CFStringRef kMenuItem_Wavetable_Sawtooth   = CFSTR ("Sawtooth");
static CFStringRef kMenuItem_Wavetable_Triangle   = CFSTR ("Triangle");

// parameter identifiers
enum {
	kParameter_Wavetable  = 0,
	kNumberOfParameters   = 1
};
// 

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

const double twopi = 2.0 * 3.14159265358979;
inline double pow5(double x) { double x2 = x*x; return x2*x2*x; }


static const CFStringRef kGlobalVolumeName = CFSTR("global volume");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TestNote : public SynthNote
{
	virtual					~TestNote() {}
	
	virtual void			Attack(const MusicDeviceNoteParams &inParams)
	{ 
		double sampleRate = SampleRate();
		phase = 0.;
		amp = 0.;
		maxamp = 0.4 * pow(inParams.mVelocity/127., 3.); 
		up_slope = maxamp / (0.1 * sampleRate);
		dn_slope = -maxamp / (0.9 * sampleRate);
		fast_dn_slope = -maxamp / (0.005 * sampleRate);
	}

	virtual Float32			Amplitude() { return amp; } // used for finding quietest note for voice stealing.
	virtual OSStatus		Render(UInt32 inNumFrames, AudioBufferList& inBufferList);
	
	int phase;
	double amp, maxamp;
	double up_slope, dn_slope, fast_dn_slope;
	
private:
	// variables ported from OSC
	float sr;
	float *ftp;
	
	// wavetable params 
	float *waveArrayPointer;
	
};

class OscilUnit : public AUMonotimbralInstrumentBase
{
	public:
	OscilUnit(ComponentInstance inComponentInstance);
				
	virtual OSStatus			Initialize();
	virtual OSStatus			Version() { return kOscilUnitVersion; }
	
	virtual OSStatus			GetParameterInfo(AudioUnitScope inScope, AudioUnitParameterID inParameterID, AudioUnitParameterInfo &outParameterInfo);
	virtual ComponentResult		GetParameterValueStrings(	AudioUnitScope					inScope,
														 AudioUnitParameterID			inParameterID,
														 CFArrayRef *					outStrings);
private:
	
	TestNote mTestNotes[kNumNotes];
};
