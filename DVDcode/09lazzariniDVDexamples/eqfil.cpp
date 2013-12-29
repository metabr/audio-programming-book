#include <stdio.h>
#include <math.h>
#include "eqfil.h"

#define PI (4.*atan(1.))

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
  return new Eqfil (audioMaster);
}

 
Eqfil::Eqfil(audioMasterCallback audioMaster)
  : AudioEffectX (audioMaster, PROGS, PARAMS)
{
  programs = new Program[numPrograms];
  gain  = .25f;
  freq = 0.1f;
  q = 0.f;
  param_changed = true;

  if (programs)
    setProgram (0);

  setNumInputs (1);	// 1 channel
  setNumOutputs (1);	// input & output
  
  setUniqueID('EqFL');	// this should be unique
  resume ();		// flush buffer
}


Eqfil::~Eqfil()
{
  if (programs) delete[] programs;
}

void Eqfil::setProgram (VstInt32 program)
{
  Program* ap = &programs[program];

  curProgram = program;
  setParameter (GAIN,ap->gain);	
  setParameter (FREQ,ap->freq);
  setParameter (QU,ap->q);
	
}

void Eqfil::setProgramName (char *name)
{
  strcpy (programs[curProgram].name, name);
}


void Eqfil::getProgramName (char *name)
{
  if (!strcmp (programs[curProgram].name, "default"))
    sprintf (name, "%s %d", programs[curProgram].name, curProgram + 1);
  else
    strcpy (name, programs[curProgram].name);
}


bool Eqfil::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
  if (index < PROGS)
    {
      strcpy (text, programs[index].name);
      return true;
    }
  return false;
}

void Eqfil::resume ()
{
  z1 = z2 = 0.0;
  AudioEffectX::resume ();
}

void Eqfil::setParameter (VstInt32 index, float value)
{
  Program &ap = programs[curProgram];

  switch (index)
    {
    case FREQ:    
      freq = ap.freq = value;    
      break;
    case GAIN:     
      gain = ap.gain = value; 
      break;
    case QU:     
      q = ap.q = value;     
    }
  param_changed = true;
}

float Eqfil::getParameter (VstInt32 index)
{
  float param = 0;

  switch (index)
    {
    case GAIN:    
      param = gain;   
      break;
    case FREQ:   
      param = freq;  
      break;
    case QU:
      param = q;
    }
  return param;
}


void Eqfil::getParameterName (VstInt32 index, char *label)
{
  switch (index)
    {
    case GAIN: 
      strcpy (label, "gain");  
      break;
    case FREQ: 
      strcpy (label, "frequency");
      break;
    case QU: 
      strcpy (label, "Q");       
    }
}


void Eqfil::getParameterDisplay (VstInt32 index, char *text)
{
  switch (index)
    {
    case FREQ:    
      float2string (pow(getSampleRate()/2., freq), text, kVstMaxParamStrLen); 
      break;
    case GAIN:     
      dB2string (gain*4, text, kVstMaxParamStrLen); 
      break;
    case QU:    
      int2string ((int)(q*100)+1, text, kVstMaxParamStrLen);
    }
}

void Eqfil::getParameterLabel (VstInt32 index, char *label)
{
  switch (index)
    {
    case FREQ:    
      strcpy (label, "Hz");  
      break;
    case GAIN: 
      strcpy (label, "dB");	   
      break;
    case QU:    
      strcpy (label, ""); 
    }
}


void Eqfil::processReplacing (float** inputs, float** outputs, VstInt32 vecframes)
{
  float* in = inputs[0];
  float* out = outputs[0];
  double w,y;
 
  if(param_changed){ 
     float sr = getSampleRate();
     double bw, c, fr = pow(sr/2., freq);
     bw = fr/(q*100.+1);
     d = cos(2*PI*fr/sr);
     c = tan(PI*bw/sr);
     a = (1.0-c)/(1.0+c);
     g = gain*4.;
     param_changed = false;
  }

  for(int i=0; i < vecframes; i++){
      w = (double) in[i] + d*(1.0 + a)*z1 - a*z2;
      y = w*a - d*(1.0 + a)*z1 + z2;
      z2 = z1;
      z1 = w;
      out[i] = (float) (0.5*(y + in[i] + g*(in[i] - y)));
    } 
}
