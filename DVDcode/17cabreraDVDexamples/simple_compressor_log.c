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

// simple_compressor_log. c
// Calculates the attack/release curve on dB values

typedef struct _mydata
{
  float threshold; // in dB FS value
  float ratio; // inverse ratio factor
  float attack_constant;
  float release_constant;
  float env; // to pass to the next block (in dB FS)
} mydata;

int data_init(mydata *p)
{
  p->threshold = (float) THRESHOLD; // in dB FS
  p->ratio = 1.0 - (1.0/(float) RATIO);  // to calculate gain reduction
  p->attack_constant = 1.0 / ((float) ATTACK_TIME * (float) SR);
  p->release_constant = 1.0 / ((float) RELEASE_TIME * (float) SR);
  p->env = 0.0; // Start at unity gain (no reduction)
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
  float indb, dest;
  float *inp = (float *) input, *outp = (float *) output;
  for(i = 0; i < frameCount; i++){
    indb = 20.0 * log10(fabs(inp[i]));
    if (indb > th) {
      dest = (th - indb) * rat; // Gain reduction target
    }
    else {
      dest = 0.0;
    }
    if (env > dest) { // attack
      env = env + (dest-env)*ac;
    }
    else { // release
      env = env + (dest-env)*rc;
    }
    outp[i] = inp[i]* pow(10, env * 0.05);
  }
  p->env = env;
}

