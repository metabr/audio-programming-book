#include <stdio.h>
#include "myplug.h"

MyPlug::MyPlug(audioMasterCallback audioMaster)
	: AudioEffectX (audioMaster, PROGS, PARAMS)
{
	programs = new MyPlugProgram[numPrograms];

	gain = 0.5;
	pitch = 1.0;
	fdb = 0.0;
	taps = 2;
	sr = getSampleRate();
	dsize = (int)(0.045 * sr);
	delay = new float[(int)sr];
	env = new float[(int)sr];

	for (int i = 0; i < dsize/2; i++)
		env[i] = i * 2.0 / dsize;
	for (int i = dsize / 2; i >= 0; i--)
		env[dsize - i - 1] = i * 2.0 / dsize;

	if (programs)
		setProgram(0);

	setNumInputs(1);
	setNumOutputs(1);
	setUniqueID('MpLg');
	resume();
}

MyPlug::~MyPlug()
{
	if (delay)
		delete[] delay;
	if (env)
		delete[] env;
	if (programs)
		delete[] programs;
}

void MyPlug::resume()
{
	wp = 0;
	rp = 0.0;
	memset(delay, 0, sr * sizeof(float));
	AudioEffectX::resume();
}

bool MyPlug::getEffectName(char *name)
{
	strcpy(name, "PitchShift");
	return true;
}

bool MyPlug::getVendorString(char *text)
{
	strcpy(text, "Aleh Suprunovich");
	return true;
}

bool MyPlug::getProductString(char *text)
{
	strcpy(text, "PitchShift");
	return true;
}

void MyPlug::setParameter(VstInt32 index, float value)
{
	MyPlugProgram &ap = programs[curProgram];
	switch (index) {
		case GAIN:
			setGain(ap.gain = value);
			break;
		case PITCH:
			setPitch(ap.pitch = value);
			break;
		case DSIZE:
			setDsize(ap.dsize = value);
			break;
		case FDB:
			setFdb(ap.fdb = value);
			break;
	}
}

float MyPlug::getParameter(VstInt32 index)
{
	float param = 0;

	switch (index) {
		case GAIN:
			param = gain;
			break;
		case PITCH:
			param = pitch;
			break;
		case DSIZE:
			param = dsize / sr;
			break;
		case FDB:
			param = fdb;
			break;
	}

	return param;
}

void MyPlug::setDsize(float d)
{
	dsize = (int)(sr * d);
	for (int i = 0; i < dsize / 2; i++)
		env[i] = i * 2.0 / dsize;
	for (int i = dsize / 2; i >= 0; i--)
		env[dsize - i - 1] = i * 2.0 / dsize;
}

void MyPlug::getParameterName(VstInt32 index, char *name)
{
	switch (index) {
		case GAIN:
			strcpy(name, "Gain");
			break;
		case PITCH:
			strcpy(name, "Pitch");
			break;
		case DSIZE:
			strcpy(name, "Delay");
			break;
		case FDB:
			strcpy(name, "Feedback");
			break;
	}
}

void MyPlug::getParameterLabel(VstInt32 index, char *label)
{
	switch (index) {
		case GAIN:
			strcpy(label, "dB");
			break;
		case PITCH:
			strcpy(label, "ratio");
			break;
		case DSIZE:
			strcpy(label, "secs");
			break;
		case FDB:
			strcpy(label, "gain");
			break;
	}
}

void MyPlug::getParameterDisplay(VstInt32 index, char *text)
{
	switch (index) {
		case GAIN:
			dB2string(gain, text, kVstMaxParamStrLen);
			break;
		case PITCH:
			float2string((pitch * 1.5 + 0.5), text, kVstMaxParamStrLen);
			break;
		case DSIZE:
			float2string(dsize/sr, text, kVstMaxParamStrLen);
			break;
		case FDB:
			float2string(fdb, text, kVstMaxParamStrLen);
			break;
	}
}

void MyPlug::setProgram(VstInt32 program)
{
	MyPlugProgram *ap = &programs[program];
	curProgram = program;
	setParameter(GAIN, ap->gain);
	setParameter(PITCH, ap->pitch);
	setParameter(FDB, ap->fdb);
}

void MyPlug::setProgramName(char *name)
{
	strcpy(programs[curProgram].name, name);
}

void MyPlug::getProgramName(char *name)
{
	if (!strcmp(programs[curProgram].name, "Init"))
		sprintf(name, "%s %d", programs[curProgram].name, curProgram + 1);
	else
		strcpy(name, programs[curProgram].name);
}

bool MyPlug::getProgramNameIndexed(VstInt32 category, VstInt32 index, char *text)
{
	if (index < PROGS) {
		strcpy(text, programs[index].name);
		return true;
	}

	return false;
}

void MyPlug::processReplacing(float **inputs, float **outputs, VstInt32 sampleFrames)
{
	float *in = inputs[0];
	float *out = outputs[0];
	int rpi, ep;
	float s = 0.0, rpf, frac, next, p = (pitch * 1.5) + 0.5;

	for (int i = 0; i < sampleFrames; i++) {
		for (int j = 0; j < taps; j++) {
			// tap position, offset
			rpf = rp + j * dsize / taps;
			if (rpf > dsize)
				rpf -= dsize;
			rpi = (int)rpf;
			frac = rpf - rpi;
			if (rpi != dsize - 1)
				next = delay[rpi + 1];
			else
				next = delay[0];

			// envelope index
			ep = rpi - wp;
			if (ep < 0)
				ep += dsize;

			s += (delay[rpi] + frac * (next - delay[rpi])) * env[ep];
		}
		// increment reader pointed and check bounds
		rp += p;
		if (rp > dsize)
			rp -= dsize;

		// feed the delay line
		delay[wp] = in[i] + s * fdb;

		// output signal
		out[i] = (s / taps) * gain;
		s = 0.f;

		// increment write pointer
		if (wp < dsize)
			wp++;
		else
			wp = 0;
	}
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	return new MyPlug(audioMaster);
}
