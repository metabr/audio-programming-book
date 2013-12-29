#include "synth.h"
#include <cmath>

const int NOTEON = 0x90; 
const int NOTEOFF = 0x80;
const int  CHNMASK = 0xf0;


AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
  return new Synth (audioMaster);
}

Synth::Synth (audioMasterCallback audioMaster)
  : AudioEffectX (audioMaster,PROGS,PARAMS)
{
  // initialize programs
  programs = new Program[PROGS];
  gain = 1.f;
  att = 0.01f;
  dec = 0.3f;
  ndx = 1.f;
  wav = 0.f;
  amp_incr = amp_decr = 0.0;
  attd = decd = cnt = 0;
  if (programs) setProgram (0);
  setNumInputs (0);  // no inputs
  setNumOutputs (2);    
  isSynth ();
  setUniqueID ('sYNt');	     
  playing = false;
  envtrig = false;
  ph = 0.0;
  twopi = 8.*atan(1.);
  resume();
}

Synth::~Synth ()
{
  if (programs) delete[] programs;
}

void Synth::setProgram (VstInt32 program)
{
  if (program < PROGS){
    Program *ap = &programs[program];
    curProgram = program;
    gain = ap->gain;
  }
}	

void Synth::setProgramName (char* name)
{
  strcpy (programs[curProgram].name, name);
}

void Synth::getProgramName (char* name)
{
  strcpy (name, programs[curProgram].name);
}

void Synth::getParameterLabel (VstInt32 index, char* label)
{
  switch (index)
    {
    case GAIN:
      strcpy (label, "dB");
      break;
   case DEC:
   case ATT:
      strcpy (label, "secs");
      break;
   case WAV:
      strcpy (label, "");
      break;   
   case NDX:
      strcpy (label, "");
      break;

    }
}

void Synth::getParameterDisplay (VstInt32 index, char* text)
{
  switch (index)
    {
    case GAIN:   
      dB2string (gain, text, kVstMaxParamStrLen);	
      break;
    case DEC:
      float2string (dec*10, text, kVstMaxParamStrLen);
      break;
    case ATT:
      float2string (att, text, kVstMaxParamStrLen);
      break;
    case WAV:
      float2string (wav, text, kVstMaxParamStrLen);
      break;
    case NDX:
      float2string (ndx, text, kVstMaxParamStrLen);
      break;
    }
}

void Synth::getParameterName (VstInt32 index, char* label)
{
  switch (index)
    {
    case GAIN:		
      strcpy (label, "gain");	
      break;
    case ATT:		
      strcpy (label, "attack");	
      break;
    case DEC:		
      strcpy (label, "decay");	
      break;
    case WAV:		
      strcpy (label, "waveform");	
      break;
    case NDX:		
      strcpy (label, "brightness");	
      break;
    }
}

void Synth::setParameter (VstInt32 index, float value)
{
  Program *ap = &programs[curProgram];
  switch (index)
    {	
    case GAIN:		
      gain = ap->gain = value;	
      break;
    case ATT:		
      att =  ap->att = value;	
      break;
    case DEC:		
      dec = ap->dec = value;	
      break;
    case WAV:		
      wav = ap->wav = value;	
      break;
    case NDX:		
      ndx = ap->ndx = value;	
      break;
    }
}

float Synth::getParameter (VstInt32 index)
{
  float value = 0;
  switch (index)
    {
    case GAIN:		
      value = gain;	
      break;
    case ATT:		
      value = att;	
      break;
    case DEC:		
      value = dec;	
      break;
    case NDX:		
      value = ndx;	
      break;
    case WAV:		
      value = wav;	
      break;
    }
  return value;
}

bool Synth::getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text)
{
  if (index < PROGS)
    {
      strcpy (text, programs[index].name);
      return true;
    }
  return false;
}

VstInt32 Synth::canDo (char* text)
{
  if (!strcmp (text, "receiveVstEvents"))
    return 1;
  if (!strcmp (text, "receiveVstMidiEvent"))
    return 1;
  
   return -1;   
}


double Synth::note2freq(int note) {
  return 440.*pow(2., (note-69)/12.);
}

float Synth::vel2amp(int vel){
  return 20*log10(vel/128.);
}

void Synth::processReplacing (float** inputs, float** outputs, VstInt32 vecframes)
{
  float* out1 = outputs[0];
  float* out2 = outputs[1];
  float fo,k,cosine,square;
  int start = 0;
  if(playing || cnt < decd)
    {       	 
      if (envtrig) {
       if (delta > 0)
	{
	  if (delta >= vecframes){
	    delta -= vecframes; return; // nothing to do now
	  }
          memset (out1, 0, delta * sizeof (float)); // zero initial portion
	  memset (out2, 0, delta * sizeof (float));  // of vector
	  }
            attd = (unsigned int) (att*getSampleRate());
            decd = (unsigned int) (dec*getSampleRate())*10;
            if(attd) amp_incr = (vel2amp(velocity) - amp)/attd;
            else amp = vel2amp(velocity);
            if(decd) amp_decr = pow(0.001, 1./decd);
            else amp_decr = 0.f;
            envtrig = false;
            cnt = 0;
            start = delta;
     }
	 
      for(int i = start; i < vecframes; i++){
	if(i==delta){
         fo = note2freq(note);
         incr = fo/getSampleRate();
         k = (ndx+0.01)*twopi*6000/(fo*log10(fo));
	}
        if(cnt < attd) amp += amp_incr;
        else if(cnt <= attd + decd)  amp *= amp_decr;        
        cosine = cos(ph*twopi);
        square = tanh(amp*k*sin(ph*twopi)/2.);
	out1[i] = amp*gain*(1.f-wav/2.f)*square*(1.+wav*cosine);
        out2[i] = out1[i];
	ph += incr;
	while(ph >= 1.0) ph -= 1.0; 
        cnt++;                  
      }
      delta = 0;
    }						
  else {
    amp = 0.f;
    memset (out1, 0, vecframes * sizeof (float));
    memset (out2, 0, vecframes * sizeof (float));
  }
}


VstInt32 Synth::processEvents (VstEvents* ev){
  for (int i=0; i < ev->numEvents; i++)
    {
      if ((ev->events[i])->type != kVstMidiType) continue;
      VstMidiEvent* event = (VstMidiEvent*)ev->events[i];
      char* data = event->midiData;
      VstInt32 status = data[0] & CHNMASK;	// OMNI
      if (status == NOTEON || status == NOTEOFF) // NOTEON or NOTEOFF
	{
	  if(status == NOTEOFF || !data[2]) {
	    if(data[1] == note) {
                 playing = false;
                if(cnt < attd) cnt = attd;
	    }
	  }
	  else {
	    note = data[1];
	    velocity = data[2];
	    delta = event[i].deltaFrames;
	    if(!playing) envtrig = true;
	    playing = true;
	  }       
	}
    }
  return 1;
}
