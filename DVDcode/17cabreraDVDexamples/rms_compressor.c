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
#define LOOKAHEAD_SAMPLES 1024
#define RMS_WINDOW_SAMPLES 2048

typedef struct _mydata
{
  float threshold; // in dB FS value
  float ratio; // inverse ratio factor
  float attack_constant;
  float release_constant;
  float env; // to pass to the next block in linear amplitude
  float delay[LOOKAHEAD_SAMPLES];
  unsigned int delay_position;
  float rms_delay[RMS_WINDOW_SAMPLES];
  unsigned int rms_delay_position;
  float rms_accumulator;
} mydata;

int data_init(mydata *p)
{
  p->threshold = (float) THRESHOLD; // in dB FS
  p->ratio = 1.0 - (1.0/(float) RATIO);  // to calculate gain reduction
  p->attack_constant = 1.0 / ((float) ATTACK_TIME * (float) SR);
  p->release_constant = 1.0 / ((float) RELEASE_TIME * (float) SR);
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
  int i;
  float th = p->threshold, rat = p->ratio;
  float ac = p->attack_constant, rc = p->release_constant;
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
    if (inrmsdb > th) {
      dest = (th - inrmsdb) * rat; // Gain reduction target, always negative
    }
    else {
      dest = 0.0;
    }
    dest = pow(10, dest * 0.05);
    if (env > dest) { //attack
      env = env + (dest-env)*ac;
    }
    else { //release
      env = env + (dest-env)*rc;
    }
    outp[i] = p->delay[p->delay_position]* env;
    p->delay[p->delay_position] = inp[i];
    if (++(p->delay_position) == LOOKAHEAD_SAMPLES)
      p->delay_position = 0;
  }
  p->env = env;
}

