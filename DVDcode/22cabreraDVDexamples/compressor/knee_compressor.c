// Values for realtime processing
#define SR 44100
#define BUF 512
#define END  60

// Threshold in dB FS
#define THRESHOLD -20
#define RATIO 7
// Attack time in seconds
#define ATTACK_TIME 0.1
// Release time in seconds
#define RELEASE_TIME 0.5
// Knee width in decibels
#define KNEE_WIDTH 8


#include <math.h>

void data_init(mydata *p)
{
//   p->threshold = pow(10, (float)THRESHOLD / 20.0); // in linear amp value
  p->threshold = (float) THRESHOLD; // in dB FS
  p->ratio = 1.0 - (1.0/(float) RATIO);  // to calculate gain reduction
  p->attack_constant = 1.0 / ((float) ATTACK_TIME * (float) SR);
  p->release_constant = 1.0 / ((float) RELEASE_TIME * (float) SR);
  p->knee = (float) KNEE_WIDTH;
  p->env = 1.0; // Start at unity gain
  memset(p->delay, 0.0, LOOKAHEAD_SAMPLES); //Clear delay line
  p->delay_position = 0;
  memset(p->rms_delay, 0.0, RMS_WINDOW_SAMPLES); //Clear rms delay line
  p->rms_delay_position = 0;
  p->rms_accumulator = 0.0;
}

void audio_process(const void *input,
                  void *output,
                  unsigned long frameCount,
                  void *userData)
{
  mydata *p = (mydata *)userData;
  unsigned long i;
//   float th = p->threshold;
  float rat, osh;
//   float ac = p->attack_constant, rc = p->release_constant;
//   float knee = p->knee;
  float env = p->env;
  float inrmsdb, dest;
  float isq;
  float *inp = (float *) input, *outp = (float *) output;
  for(i = 0; i < frameCount; i++){
    isq = inp[i] * inp[i];
    p->rms_accumulator += isq;
    p->rms_accumulator -= p->rms_delay[p->rms_delay_position];
    p->rms_delay[p->rms_delay_position] = isq;
    if (++(p->rms_delay_position) == RMS_WINDOW_SAMPLES)
      p->rms_delay_position = 0;
    inrmsdb = 20.0 * log10(sqrt(p->rms_accumulator));
    if (inrmsdb > p->threshold - (p->knee/2.0) ) {
      osh = inrmsdb - (p->threshold - (p->knee/2.0));
      if (osh < p->knee) {
        rat = ((osh * (p->ratio - 1))/p->knee) + 1;
      }
      else {
        rat = p->ratio;
      }
      dest = -osh * rat; // Gain reduction target, always negative
    }
    else {
      dest = 0.0;
    }
    dest = pow(10, dest * 0.05);
    if (env > dest) { //attack
      env = env + (dest-env)*p->attack_constant;
    }
    else { //release
      env = env + (dest-env)*p->release_constant;
    }
    outp[i] = p->delay[p->delay_position]* env;
    p->delay[p->delay_position] = inp[i];
    if (++(p->delay_position) == LOOKAHEAD_SAMPLES)
      p->delay_position = 0;
  }
  p->env = env;
}
