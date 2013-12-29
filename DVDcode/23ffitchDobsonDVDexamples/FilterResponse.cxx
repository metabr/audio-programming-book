/*
Copyright (c) 2009 John ffitch
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// generated in part by Fast Light User Interface Designer (fluid) version 1.0106

#include "FilterResponse.h"
#include <math.h>
#include <FL/fl_draw.H>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sndfile.h>
#include <portaudio.h>


#define ppi  3.14159265358979
// oscillator code from RWD Book chapter 2
typedef struct  t_oscil 
{
	double srate;
	double twopiovrsr;
	double curfreq;
	double curphase;
	double incr;
} OSCIL;
OSCIL*  new_oscil(double srate);
double sinetick  (OSCIL* p_osc, double freq);
OSCIL *osc = 0;


int STEPS = 1000;
int DBMIN = 60;
double *f;
double *ff;
double *p;
static void do_options(Fl_Button*, void*);
static void do_sound(Fl_Button*, void*);

Fl_Value_Input *A0=(Fl_Value_Input *)0;

Fl_Value_Input *A1=(Fl_Value_Input *)0;

Fl_Value_Input *A2=(Fl_Value_Input *)0;

//Fl_Value_Input *B0=(Fl_Value_Input *)0;

Fl_Value_Input *B1=(Fl_Value_Input *)0;

Fl_Value_Input *B2=(Fl_Value_Input *)0;

Fl_Check_Button *dbscale=(Fl_Check_Button *)0;

Fl_Check_Button *logscale=(Fl_Check_Button *)0;

Fl_Light_Button *play_button=(Fl_Light_Button *)0;

static void cb_Quit(Fl_Button*, void*) {
  exit(0);
}

struct Fl_FGraph :  Fl_Window {
  Fl_FGraph(int x,int y,const char* l=0): Fl_Window(x,y,l){}
  void draw();
  int handle(int);
};

Fl_FGraph *fgraph = (Fl_FGraph *)0;

static void cb_db(Fl_Button*, void*) {
  if (fgraph->visible()) {
    fgraph->redraw();
  }
}

struct Fl_PGraph :  Fl_Window {
  Fl_PGraph(int x, int y, const char* l=0): Fl_Window(x,y,l){}
  void draw();
  int handle(int);
};

Fl_PGraph *pgraph = (Fl_PGraph *)0;
static int changed = 1;

static void cb_log(Fl_Button*, void*)
{
    changed = 1;
    do_calc();
    if (fgraph->visible()) {
      fgraph->redraw();
    }
    if (pgraph->visible()) {
      pgraph->redraw();
    }
}

struct Fl_ZGraph :  Fl_Window {
  Fl_ZGraph(int x, int y, const char* l=0): Fl_Window(x,y,l){}
  void draw();
};

Fl_ZGraph *zgraph = (Fl_ZGraph *)0;

struct Fl_TGraph :  Fl_Window {
  Fl_TGraph(int x, int y, const char* l=0): Fl_Window(x,y,l){}
  void draw();
};

Fl_TGraph *tgraph = (Fl_TGraph *)0;

static void cb_AB(Fl_Value_Input*, void*)
{
    changed = 1;
    do_calc();
    if (fgraph->visible()) fgraph->redraw();
    if (pgraph->visible()) pgraph->redraw();
    if (zgraph->visible()) zgraph->redraw();
    if (tgraph->visible()) tgraph->redraw();
}

#define TSTEPS (1024)
double x[TSTEPS];
double y[TSTEPS];
int valid_data = 0;

Fl_Output *xout=(Fl_Output *)0;

Fl_Output *yout=(Fl_Output *)0;

double Default[5] = {1.0, 0.0, 0.0, 0.0, 0.0};

Fl_Window* make_window()
{
    f = new double[STEPS];
    ff = new double[STEPS];
    p = new double[STEPS];
  Fl_Window* w;
  { Fl_Window* o = new Fl_Window(220, 185, "Filter Control Panel");
    w = o;
    o->box(FL_UP_BOX);
    { Fl_Value_Input* o = A0 = new Fl_Value_Input(25, 5, 80, 20, "a0");
      o->minimum(0.0000001);
      o->maximum(5);
      o->step(0.001);
      o->value(Default[0]);
      o->callback((Fl_Callback*)cb_AB);
    }
    { Fl_Value_Input* o = A1 = new Fl_Value_Input(25, 30, 80, 20, "a1");
      o->minimum(-5);
      o->maximum(5);
      o->step(0.001);
      o->value(Default[1]);
      o->callback((Fl_Callback*)cb_AB);
    }
    { Fl_Value_Input* o = A2 = new Fl_Value_Input(25, 55, 80, 20, "a2");
      o->minimum(-5);
      o->maximum(5);
      o->step(0.001);
      o->value(Default[2]);
      o->callback((Fl_Callback*)cb_AB);
    }
//     { Fl_Value_Input* o = B0 = new Fl_Value_Input(25, 90, 80, 20, "b0");
//       o->minimum(0.001);
//       o->maximum(5);
//       o->step(0.001);
//       o->value(1);
//       o->callback((Fl_Callback*)cb_AB);
//     }
    { Fl_Value_Input* o = B1 = new Fl_Value_Input(25, 115, 80, 20, "b1");
      o->minimum(-5);
      o->maximum(5);
      o->step(0.001);
      o->value(Default[3]);
      o->callback((Fl_Callback*)cb_AB);
    }
    { Fl_Value_Input* o = B2 = new Fl_Value_Input(25, 140, 80, 20, "b2");
      o->minimum(-5);
      o->maximum(5);
      o->step(0.001);
      o->value(Default[4]);
      o->callback((Fl_Callback*)cb_AB);
    }
    { Fl_Check_Button* o = logscale =
        new Fl_Check_Button(130, 5, 75, 20, "log scale");
      o->tooltip("Use log scale for frequency");
      o->down_box(FL_DOWN_BOX);
      o->callback((Fl_Callback*)cb_log);
    }
    { Fl_Button* o = new Fl_Button(130, 25, 75, 20, "Magnitude");
      o->tooltip("Draw magnitude response curve");
      o->color(FL_WHITE);
      o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
      o->callback((Fl_Callback*)do_freq);
    }
    { Fl_Check_Button* o = dbscale =
        new Fl_Check_Button(130, 45, 75, 20, "dB scale");
      o->tooltip("Use log scale or linear scale");
      o->down_box(FL_DOWN_BOX);
      o->callback((Fl_Callback*)cb_db);
    }
    { Fl_Button* o = new Fl_Button(130, 65, 75, 20, "Phase");
      o->tooltip("Draw phase response curve");
      o->color(FL_LIGHT3);
      o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
      o->callback((Fl_Callback*)do_phase);
    }
    { Fl_Button* o = new Fl_Button(130, 90, 75, 20, "Z-Plane");
      o->tooltip("Display z-plane with poles and zeros");
      o->color(FL_LIGHT2);
      o->callback((Fl_Callback*)do_zplane);
    }
    { Fl_Button* o = new Fl_Button(130, 115, 75, 20, "Impulse");
      o->tooltip("Display z-plane with poles and zeros");
      o->color(FL_LIGHT1);
      o->callback((Fl_Callback*)do_impulse);
    }
    { Fl_Button* o = new Fl_Button(130, 140, 75, 20, "Quit");
      o->color(95);
      o->align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
      o->callback((Fl_Callback*)cb_Quit);
    }
    { Fl_Output* o = xout = new Fl_Output(25, 165, 65, 15);
      o->box(FL_FLAT_BOX);
      o->color(29);
      o->textsize(10);
    }
    { Fl_Output* o = yout = new Fl_Output(130, 165, 85, 15);
      o->box(FL_FLAT_BOX);
      o->color(29);
      o->textsize(10);
    }
    { Fl_Button* o = new Fl_Button(95, 85, 30, 15, "options");
      o->color(FL_FOREGROUND_COLOR);
      o->labelsize(12);
      o->align(FL_ALIGN_LEFT);
      o->callback((Fl_Callback*)do_options);
    }
    { Fl_Button* o = new Fl_Button(5, 85, 40, 15, "sound");
      o->color(/*(Fl_Color)30*/FL_LIGHT3);
      o->labelsize(12);
      o->callback((Fl_Callback*)do_sound);
    }
    o->end();
  }
  return w;
}

Fl_FGraph* make_box1() {
  Fl_FGraph* w;
  { Fl_FGraph* o = new Fl_FGraph(380, 149, "Magnitude Response");
    o->color(FL_WHITE);
    w = o;
    o->end();
    o->resizable(o);
  }
  return w;
}

Fl_PGraph* make_box2() {
  Fl_PGraph* w;
  { Fl_PGraph* o = new Fl_PGraph(380, 149, "Phase Response");
    o->color(FL_WHITE);
    w = o;
    o->end();
    o->resizable(o);
  }
  return w;
}

Fl_ZGraph* make_box3() {
  Fl_ZGraph* w;
  { Fl_ZGraph* o = new Fl_ZGraph(240, 240, "Z Plane");
    o->color(FL_WHITE);
    w = o;
    o->end();
    o->resizable(o);
  }
  return w;
}

Fl_TGraph* make_box4() {
  Fl_TGraph* w;
  { Fl_TGraph* o = new Fl_TGraph(380, 149, "Impulse Response");
    o->color(FL_WHITE);
    w = o;
    o->end();
    o->resizable(o);
  }
  return w;
}

Fl_Window *options=(Fl_Window *)0;

Fl_Window *sound=(Fl_Window *)0;

Fl_Counter *resolution=(Fl_Counter *)0;

Fl_Counter *mindb=(Fl_Counter *)0;

Fl_Counter *Minf=(Fl_Counter *)0;

Fl_Counter *Maxf=(Fl_Counter *)0;

Fl_Counter *interval=(Fl_Counter *)0;

static void cb_resolve(Fl_Counter*x, void*)
{
    STEPS = (int)x->value();
    changed = 1;
    delete f; delete ff; delete p; // Should delete old values
    f = new double[STEPS];
    ff = new double[STEPS];
    p = new double[STEPS];
    do_calc();
    if (fgraph->visible()) fgraph->redraw();
    if (pgraph->visible()) pgraph->redraw();
}

static void cb_dbscale(Fl_Counter*x, void*)
{
    DBMIN = -(int)x->value();
    if (dbscale->value() && fgraph->visible()) fgraph->redraw();
}

// static void cb_OK(Fl_Button*, void*)
// {
//     options->hide();
// }

Fl_Window* make_options() {
  Fl_Window* w;
  { Fl_Window* o = options = new Fl_Window(182, 170 /* 103 */, "Options");
    w = o;
    { Fl_Counter* o = resolution = new Fl_Counter(75, 5, 95, 20, "resolution:");
      o->minimum(100);
      o->maximum(9999);
      o->step(1);
      o->lstep(100);
      o->value(STEPS);
      o->callback((Fl_Callback*)cb_resolve);
      o->align(FL_ALIGN_LEFT);
    }
    { Fl_Counter* o = mindb = new Fl_Counter(75, 40, 95, 20, "min dB:");
      o->minimum(-150);
      o->maximum(-6);
      o->step(1);
      o->lstep(6);
      o->value(-DBMIN);
      o->callback((Fl_Callback*)cb_dbscale);
      o->align(FL_ALIGN_LEFT);
    }
    { Fl_Counter* o = Minf = new Fl_Counter(75, 75, 95, 20, "min freq:");
      o->minimum(0);
      o->maximum(22050);
      o->step(1);
      o->lstep(100);
      o->value(50);
      o->align(FL_ALIGN_LEFT);
    }
    { Fl_Counter* o = Maxf = new Fl_Counter(75, 110, 95, 20, "max freq:");
      o->minimum(0);
      o->maximum(22050);
      o->step(1);
      o->lstep(100);
      o->value(18000);
      o->align(FL_ALIGN_LEFT);
    }
    { Fl_Counter* o = interval = new Fl_Counter(75, 145, 95, 20, "interval:");
      o->minimum(5);
      o->maximum(120);
      o->step(0.25);
      o->lstep(5);
      o->value(30);
      // no callback needed - we simply read the value
      o->align(FL_ALIGN_LEFT);
    }
//     { Fl_Button* o = new Fl_Button(115, 70, 25, 25, "OK");
//       o->color((Fl_Color)3);
//       o->callback((Fl_Callback*)cb_OK);
//     }
    o->end();
  }
  return w;
}

/////////////////////////
// The playback section//
/////////////////////////

Fl_Window *Player=(Fl_Window *)0;
SNDFILE *now_playing = NULL;
SF_INFO info;
Fl_Round_Button* fbutton, *sbutton, *cbutton;
Fl_File_Input *play_name;
int playing = 0;
int chans = 0;
int unstable = 0;
void do_play();
void do_sine(int);

static PaStream *stream;
static void cb_play(Fl_Light_Button* x, void*)
{
    if (playing||unstable) {
      play_button->value(0);
      if (fbutton->value()) sf_close(now_playing);
      Pa_StopStream(stream);
      Pa_CloseStream(stream);
      playing = 0;
      fbutton->activate();
      sbutton->activate();
      cbutton->activate();
    }
    else {
      playing = 1;
      fbutton->deactivate();
      sbutton->deactivate();
      cbutton->deactivate();
      if (fbutton->value()) {   // Play file through filter
        //        printf("File=%s\n", play_name->value());
        now_playing = sf_open(play_name->value(), SFM_READ, &info);
        if (now_playing==NULL) {
          playing = 0;
          fbutton->activate();
          sbutton->activate();
          cbutton->activate();
          x->value(0);
        }
        else {
          // Start playing process
          chans = info.channels;
          do_play();
        }
      }
      else if (sbutton->value()) { // swept sine
        do_sine(0);
      }
      else {                    // linear swept sine
        do_sine(1);
      }
    }
}


static float fx1=0.0f, fx2=0.0f, fy1=0.0f, fy2=0.0f;
inline float filter(float x0)
{
  //    static float x1=0.0f, x2=0.0f, y1=0.0f, y2=0.0f;
    float y0 = x0*(float)A0->value() + 
               fx1*(float)A1->value() + 
               fx2*(float)A2->value()
            -fy1*(float)B1->value() -fy2*(float)B2->value();
    fy2 = fy1; fy1 = y0; fx2 = fx1; fx1 = x0;
    return y0;
}


#define	BUFFER_LEN			(1024)
static float abuffer [BUFFER_LEN];
static float fbuffer [2*BUFFER_LEN];
int bufflen = 0;
static int paCallback(const void *inputBuffer,
                      void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData )
{
    float *out = (float*)outputBuffer;
    unsigned long i;
    int loop = 0;               // Just in case stop infinite loop

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;

    if (bufflen==0 || unstable) {
      play_button->value(0);
      Pa_CloseStream(stream);
      Pa_Terminate();
      fbutton->value(0);
      return -1;
    }
    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = filter(abuffer[i]);  /* left */
    }
 retry:
    if (chans==1) {
      bufflen = sf_read_float(now_playing, abuffer, BUFFER_LEN);
    }
    else {
      bufflen = sf_readf_float(now_playing, fbuffer, BUFFER_LEN);
      for (i=0; i<BUFFER_LEN; i++)
        abuffer[i] = fbuffer[i+i];
    }
    if (bufflen==0) {
      sf_close(now_playing);
      loop++;
      now_playing = sf_open(play_name->value(), SFM_READ, &info);
      if (loop>1) {
        Pa_CloseStream(stream);
        Pa_Terminate();
        fbutton->value(0);
        return -1;
      }
      goto retry;
    }
    return paContinue;
}

void do_play()
{
//    PaStreamParameters outputParameters;
    PaError err;
    int i;
    if (chans==1) {
      bufflen = sf_read_float(now_playing, abuffer, BUFFER_LEN);
    }
    else {
      bufflen = sf_readf_float(now_playing, fbuffer, BUFFER_LEN);
      for (i=0; i<bufflen; i++)
        abuffer[i] = fbuffer[i+i];
    }
    err = Pa_Initialize();
    if( err != paNoError ) goto error;
    err = Pa_OpenDefaultStream( &stream,
                                0,          /* no input channels */
                                1,          /* mono output */
                                paFloat32,  /* 32 bit floating point output */
                                44100,
                                BUFFER_LEN, /* frames per buffer, i.e. the number
                                               of sample frames that PortAudio will
                                               request from the callback. Many apps
                                               may want to use
                                               paFramesPerBufferUnspecified, which
                                               tells PortAudio to pick the best,
                                               possibly changing, buffer size.*/
                                paCallback,
                                NULL );

    if( err != paNoError ) {
    error:
      Pa_Terminate();
      playing = 0;
      return;
    }
    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error;
}

Fl_Window* make_player() {
  Fl_Window* w;
  { Fl_Window* o = Player = new Fl_Window(305, 135, "Sound Controls");
    w = o;
    play_name = new Fl_File_Input(45, 20, 220, 35, "Audio");
    {
      Fl_Light_Button *o =
        play_button = new Fl_Light_Button(105, 100, 90, 30, "Play Audio");
      o->callback((Fl_Callback*)cb_play);
    }
    { Fl_Group* o = new Fl_Group(10, 55, 275, 45);
      { Fl_Round_Button* o = fbutton =
          new Fl_Round_Button(15, 55, 25, 45, "File");
        o->down_box(FL_ROUND_DOWN_BOX);
        o->type(FL_RADIO_BUTTON);
      }
      { Fl_Round_Button* o = sbutton = 
          new Fl_Round_Button(70, 65, 20, 25, "Log Sweep");
        o->down_box(FL_ROUND_DOWN_BOX);
        o->type(FL_RADIO_BUTTON);
        o->value(1);
      }
      { Fl_Round_Button* o = cbutton = 
          new Fl_Round_Button(170, 65, 25, 25, "Linear Sweep");
        o->down_box(FL_ROUND_DOWN_BOX);
        o->type(FL_RADIO_BUTTON);
      }
      o->end();
    }
    o->end();
  }
  return w;
}

double Time = 30.0;
double minf = 50.0, maxf = 18000.0;
double tt = 0.0;
double freq = -1.0;
//FILE *ffff = 0;

#define TWOPI (6.28318530718)

static int paCallsine(const void *inputBuffer,
                      void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData )
{
    float *out = (float*)outputBuffer;
    unsigned long i;
    double lambda;
    Time = interval->value();
    maxf = Maxf->value();
    minf = Minf->value();
    lambda = minf*exp(tt*log(maxf/minf)/Time);
    freq = lambda/44100.0;
    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;
    
    if (playing==0||unstable) {
      Pa_CloseStream(stream);
      Pa_Terminate();
      sbutton->value(0);
      // freq = -1.0;
      return -1;
    }
    if (zgraph->visible()) {
      zgraph->redraw();
    }
//    printf("freq=%f lambda=%f tt=%f lambda*tt= %f\n", freq, lambda,tt,lambda*tt);
    for( i=0; i<BUFFER_LEN; i++ ) {
      //float v = filter(0.25f*(float)sin(lambda*tt));
        float v = filter(0.25f*(float) sinetick(osc,lambda));
//            fwrite(&v, sizeof(float), 1, ffff);

      out[i] = v;
      tt += 1.0/44100.0;
      lambda = minf*exp(tt*log(maxf/minf)/Time);
    }
    if (tt>Time) {
      tt = 0.0;
    }
    return paContinue;
}


static int paCalllsine(const void *inputBuffer,
                      void *outputBuffer,
                      unsigned long framesPerBuffer,
                      const PaStreamCallbackTimeInfo* timeInfo,
                      PaStreamCallbackFlags statusFlags,
                      void *userData )
{
    float *out = (float*)outputBuffer;
    unsigned long i;
    double lambda;
    Time = interval->value();  // secs
    maxf = Maxf->value();
    minf = Minf->value();
    lambda = minf+ (maxf -minf) * tt/Time;
    freq = lambda/44100.0;

    (void) timeInfo; /* Prevent unused variable warnings. */
    (void) statusFlags;
    (void) inputBuffer;
    
    if (playing==0||unstable) {
      Pa_CloseStream(stream);
      Pa_Terminate();
      cbutton->value(0);
      // freq = -1.0;
      return -1;
    }
    if (zgraph->visible()) {
      zgraph->redraw();
    }
//   printf("freq=%f lambda=%f tt=%f lamda*tt= %f\n", freq, lambda,tt,lambda * tt);
  
    for( i=0; i<BUFFER_LEN; i++ ) {
//      float v = filter(0.25f*(float)sin( lambda  *tt));
        float v = filter(0.25f*(float) sinetick(osc,lambda));
//            fwrite(&v, sizeof(float), 1, ffff);
      out[i] = v;
      tt += 1.0/44100.0;
      lambda = minf + (maxf - minf) * tt / Time;
    }
    if (tt>Time) {
      tt = 0.0;
    }
    return paContinue;
}

void do_sine(int type)
{
  PaError err;
  int i;
// ffff = fopen("sin.raw", "wb");
  err = Pa_Initialize();
  if( err != paNoError ) goto error;
  err = Pa_OpenDefaultStream( &stream,
                              0,          /* no input channels */
                              1,          /* mono output */
                              paFloat32,  /* 32 bit floating point output */
                              44100,
                              BUFFER_LEN, /* frames per buffer, i.e. the number
                                             of sample frames that PortAudio will
                                             request from the callback. Many apps
                                             may want to use
                                             paFramesPerBufferUnspecified, which
                                             tells PortAudio to pick the best,
                                             possibly changing, buffer size.*/
                              (type==0?paCallsine:paCalllsine),
                              NULL );
  if( err != paNoError ) {
  error:
    Pa_Terminate();
    playing = 0;
    return;
  }
  /* RWD: reset sweep*/
  tt = 0;
  err = Pa_StartStream( stream );
  if( err != paNoError ) goto error;
}


double a0, a1, a2, b1, b2;

void f_p_response(double w, double *af, double *ap)
// {
//     double   t9, t13, t15, t16, t17, t18, t19, t20, t21,
//              t22, t27, t3, t30, t32, t34, t36, t41, 
//       t42, t51, t53, t54, t55, t56, t57, t58, t59;
//     t3  = cos(w);
//     t42 = t3*t3;
//     t16 = t42*b2;
//     t32 = b1*t3;
//     t58 = 4.0*t16;
//     t55 = t58+2*(t32+t32*b2);
//     t57 = b2*b2+b1*b1;
//     t13 = t57+t55+1.0-(2.0*b2);
//     t27 = t42*a0;
//     t15 = t27*a2;
//     t17 = b2*a2;
//     t18 = b1*a2;
//     t19 = a1*b2;
//     t20 = b1*a1;
//     t21 = b2*a0;
//     t22 = b1*a0;
//     t34 = a1*t3;
//     t41 = t42*t3;
//     t30 = t17*a0;
//     t36 = t16*a0;
//     t51 = 2.0*(t34*a2+t34*a0+2.0*t15-(a2*a0));
//     t53 = t34+t22*t3;
//     t54 = -(t18*t3)-(t19*t3);
//     t56 = a2*a2+a1*a1+a0*a0;
//     t59 = 2.0*a2;
//     *af=sqrt(t56+t51+t18*t18+t22*t22+
//              4.0*(t30+4.0*(t41*t30*t3-(t15*b2))+t54*a0)+
//              t56*t55+t57*t51+t17*(t17-t59-(4.0*t53))+t19*(t19-(2.0*a1))+
//              t20*(t20+4.0*(t27+t36))+t21*(t21-(2.0*a0))+
//              t41*t17*8.0*(t22+a1)+t27*t3*8.0*(t19+t18)+
//              t18*a1*(t58+4.0*t42))/fabs(t13);
//     t9=sin(w);
//     *ap=atan2(t9*(t22-a1+t19-t18)+t9*t3*(2.0*t21-t59)/t13,
//               (t53+t17+t20-t21+2.0*t36+a0-a2+t59*t42-t54)/t13);
// }
{
    double t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12,t13,t14,t15,t16,t17,t18,
      t19,t20,t21,t22,t23,t24,t45,t47,t49,t51,t53;
    t1=cos(w);
    t2=t1*t1;
    t3=t2*b2;
    t4=b1*t1;
    t5=2*(t4+t4*b2);
    t6=4*t3;
    t7=t6+t5;
    t8=b2*b2+b1*b1;
    t9=t2*a0;
    t11=t9*a2;
    t12=a2*t1;
    t14=b2*a2;
    t15=b1*a2;
    t16=a1*b2;
    t17=b1*a1;
    t18=b2*a0;
    t19=b1*a0;
    t10=a1*t1;
    t13=t2*t1;
    t20=t14*a0;
    t21=t3*a0;
    t22=t16*t1;
    t23=t12*b1;
    t24=a2*a2;
    t45=2*(t12*a1+t10*a0+2*t11-(a2*a0));
    t47=-(2*t12)-a1;
    t49=a1*a1+a0*a0;
    t51=4*t12;
    t53=2*a2;
    *af=sqrt(t49+t45+t24+t15*t15+4.0*(t20+4*t13*t20*t1-(t22*a0))+
             t49*t7+t8*t45+b2*(t51*t12-(16*t11))+t14*(t14-t53)+
             t16*(2.0*t47+t16)+t17*(t17+4.0*(t9+t21))+
             t18*(t18-(4.0*t23)-(2.0*a0))+t19*(t19-t51)+
             t13*t14*8.0*(t19+a1)+t9*t1*8.0*(t16+t15)+
             t15*a1*(t6+4.0*t2)+t5*t24)/fabs(t8+t7-(2.0*b2)+1);
    *ap=atan2(sin(w)*(t47+t19+t16-t15+2.0*t18*t1),
              t14+t17-t18+t23+t10+t22+2.0*t21+a0-a2+t19*t1+t53*t2);
}

int Fl_FGraph::handle(int event)
{
  switch(event) {
    case FL_PUSH:
      {
        int xi = Fl::event_x();
        double xx = 22050.0*(double)xi/(double)w();
        double ind = (double)(xi)/(double)w();
        double yy = f[(int)(ind*STEPS)];
        char xs[30], ys[30];
        if (logscale->value()) {
          //          printf("ind=%f  ", ind);
          xx = pow(2204,(double)ind)/2204;
          //          printf("w=%f\n", xx);
          xx = 22050.0*xx;
          //          printf("w=%f\n", xx);
        }
//      printf("xi=%d yi=%d xx=%f ind=%f yy=%f\n", xi, Fl::event_y(), xx,ind,yy);
        if (dbscale->value()) {
          yy = 20.0*log10(yy);
          sprintf(xs,"%.1f Hz",xx); sprintf(ys,"%.4f db",yy);
        }
        else {
          sprintf(xs,"%.1f Hz",xx); sprintf(ys,"%.4f",yy);
        }
        xout->value(xs), yout->value(ys);
      }
      return 1;
    default:
      return Fl_Widget::handle(event);
  }
}

void Fl_FGraph::draw()
{
    int db = dbscale->value();
    Fl_Window::draw();
    if (valid_data) {
      int i;
      double x0,y0;
      double fmax = 0.0;
      if (db) {
        for (i=0; i<STEPS; i++) ff[i] = 20.0*log10(f[i])+DBMIN;
      }
      else for (i=0; i<STEPS; i++) ff[i] = f[i];
      fl_line_style(FL_SOLID);
      fl_color(FL_BLACK);
      for (i=0; i<STEPS; i++) {
        if (ff[i]>fmax) fmax = ff[i];
      }
      double xbase = 0.0;
      double width = (double)w();
      double height = (double)h();
      double xscale = width/((double)STEPS);
      double yscale;
      //       fprintf(stderr,"w h = %f %f\nxscale=%f fmax=%f\n",
      //               width, height, xscale, fmax);
      yscale = (height-6.0)/fmax;
      if (db && fmax>0.0) {
        fl_line_style(FL_DOT);
        fl_color(FL_BLACK);
        fl_line(0, (int)(yscale*(fmax-DBMIN))+3,w(),(int)(yscale*(fmax-DBMIN))+3);
        fl_font(FL_TIMES, 10); fl_draw("0db",3,7+(int)(yscale*(fmax-DBMIN)));
//         fl_color(FL_YELLOW);
//         fl_line(0, h()+(int)((6.0-DBMIN)*yscale)-3,
//                 w(),h()+(int)((6.0-DBMIN)*yscale)-3);
//         fl_color(FL_BLACK);
        {
          char buf[100];
          sprintf(buf, "-%ddb", DBMIN);
          fl_draw(buf,3,h()-7);
        }
        fl_line_style(FL_SOLID);
      }
      else
        if (fmax>1.0) {
          fl_line_style(FL_DOT);
          fl_color(FL_BLACK);
          fl_line(0, h()-(int)yscale+3,w(),h()-(int)yscale+3);
          fl_font(FL_TIMES, 10); fl_draw("1",2,h()-(int)yscale);
          fl_line_style(FL_DOT);
          fl_line_style(FL_SOLID);
          fl_line(0, h()-3,w(),h()-3);
          fl_draw("0",2,h());
        }
      // Draw ticks???
      {
        fl_color(FL_BLUE);
        if (logscale->value()) {
#ifdef JPFF
          double ticks[] = {0.639831356713,0.729873517535,0.782544805099,
                            0.819915678357,0.848902779649,0.872586965921,
                            0.892611660089,0.909957839178,0.925258253486,
                            0.938944940471,0.95132605487,0.962629126743,
                            0.973026944925,0.98265382091,0.991616228036};

          for (i=0; i<15; i++) {
            double x = ticks[i];
            fl_line((int)(x*w()),h(),(int)(x*w()),h()-5);
          }
#else
          int ticks[] = {90,180,233,270,299,389,479,532,569,598,688,778,830,868,897,987};
          double x;
          double invsteps = 1.0 / STEPS;
          for(i=0;i < 4;i++){
              x = ticks[i] * invsteps;
              fl_line((int)(x*w()),h(),(int)(x*w()),h()-5);
          }
          x = ticks[i++] * invsteps;
          fl_color(FL_BLACK);
          fl_line_style(FL_SOLID,2);
          fl_line((int)(x*w()),h(),(int)(x*w()),h()-7);
          fl_color(FL_BLUE);
          fl_line_style(FL_SOLID,1);
          for(;i < 9;i++){
              double x = ticks[i] * invsteps;
              fl_line((int)(x*w()),h(),(int)(x*w()),h()-5);
          }
          fl_color(FL_BLACK);
          x = ticks[i++] * invsteps;
          fl_line_style(FL_SOLID,2);
          fl_line((int)(x*w()),h(),(int)(x*w()),h()-7);
          fl_color(FL_BLUE);
          fl_line_style(FL_SOLID,1);
          for(;i < 14;i++){
              double x = ticks[i] * invsteps;
              fl_line((int)(x*w()),h(),(int)(x*w()),h()-5);
          }
          fl_color(FL_BLACK);
          x = ticks[i++] * invsteps;
          fl_line_style(FL_SOLID,2);
          fl_line((int)(x*w()),h(),(int)(x*w()),h()-7);
          fl_color(FL_BLUE);
          fl_line_style(FL_SOLID,1);
          fl_line((int)(x*w()),h(),(int)(x*w()),h()-7);
#endif          
        }
        else {
          for (i=0; i<9; i++) 
            fl_line(i*w()/8,h(),i*w()/8,h()-5);
        }
        fl_color(FL_BLACK);
      }
      x0 = xbase; y0 = 3.0+ff[0]*yscale;
      for (i=1; i<STEPS; i++) {
        double x1, y1;
        x1 = x0 + xscale;
        y1 = 3.0 + ff[i]*yscale;
        fl_line((int)x0,h()-(int)y0,(int)x1,h()-(int)y1);
        x0 = x1; y0 = y1;
      }
    }
    else fprintf(stderr, "no data\n");
}

int Fl_PGraph::handle(int event)
{
  switch(event) {
    case FL_PUSH:
      {
        int xi = Fl::event_x();
        double xx = 22050.0*(double)xi/(double)w();
        double yy = p[xi*STEPS/w()];
        char xs[30], ys[30];
        if (logscale->value()) {
          double ind = (double)xi/(double)w();
          //          printf("ind=%f  ", ind);
          //          printf("ind=%f  ", ind);
          //xx = 22050.0*(1.0+log(0.5*(double)(ind+1))/log(2.0));
          //xx = 22050.0*(1.0+log((double)(ind))/log(2.0));
          xx = 22050.0*pow(2204.0,(double)ind)/2204.0;
          //          printf("w=%f\n", xx);
        }
        sprintf(xs,"%.1f Hz",xx); sprintf(ys,"%.4f radians",yy);
        xout->value(xs), yout->value(ys);
      }
      return 1;
    default:
      return Fl_Widget::handle(event);
  }
}

void Fl_PGraph::draw()
{
    Fl_Window::draw();
    if (valid_data) {
       int i;
       double dy=0.0;
       double x0,y0;
       //       fprintf(stderr,"in draw\n");
       double fmax = 0;
       double xbase = 0.0;
       double width = (double)w();
       double height = (double)h();
       double ybase = height*0.5;
       double xscale = width/((double)STEPS);
       double yscale = height/(ppi+ppi);
       fl_line_style(FL_DOT);
       fl_color(FL_BLACK);
       fl_line(0,(int)ybase,w(),(int)ybase);
       fl_color(FL_YELLOW);
       fl_line(0,(int)(ppi*0.5*yscale+ybase),w(),(int)(ppi*0.5*yscale+ybase));
       fl_line(0,(int)(-ppi*0.5*yscale+ybase),w(),(int)(-ppi*0.5*yscale+ybase));
       fl_line_style(FL_SOLID);
       { // ticks
        fl_color(FL_BLUE);
        if (logscale->value()) {
#ifdef JPFF
          double ticks[] = {0.639831356713,0.729873517535,0.782544805099,
                            0.819915678357,0.848902779649,0.872586965921,
                            0.892611660089,0.909957839178,0.925258253486,
                            0.938944940471,0.95132605487,0.962629126743,
                            0.973026944925,0.98265382091,0.991616228036};

          for (i=0; i<15; i++) {
            double x = ticks[i];
            fl_line((int)(x*w()),h(),(int)(x*w()),h()-5);
          }
#else
          int ticks[] = {90,180,233,270,299,389,479,532,569,598,688,778,830,868,897,987};
          double x;
          double invsteps = 1.0 /STEPS;
          for(i=0;i < 4;i++){
              x = ticks[i] * invsteps;
              fl_line((int)(x*w()),h(),(int)(x*w()),h()-5);
          }
          x = ticks[i++] * invsteps;
          fl_color(FL_BLACK);
          fl_line_style(FL_SOLID,2);
          fl_line((int)(x*w()),h(),(int)(x*w()),h()-7);
          fl_color(FL_BLUE);
          fl_line_style(FL_SOLID,1);
          for(;i < 9;i++){
              double x = ticks[i] * invsteps;
              fl_line((int)(x*w()),h(),(int)(x*w()),h()-5);
          }
          fl_color(FL_BLACK);
          x = ticks[i++] * invsteps;
          fl_line_style(FL_SOLID,2);
          fl_line((int)(x*w()),h(),(int)(x*w()),h()-7);
          fl_color(FL_BLUE);
          fl_line_style(FL_SOLID,1);
          for(;i < 14;i++){
              double x = ticks[i] * invsteps;
              fl_line((int)(x*w()),h(),(int)(x*w()),h()-5);
          }
          fl_color(FL_BLACK);
          x = ticks[i++] * invsteps;
          fl_line_style(FL_SOLID,2);
          fl_line((int)(x*w()),h(),(int)(x*w()),h()-7);
          fl_color(FL_BLUE);
          fl_line_style(FL_SOLID,1);
#endif
        }
        else {
          for (i=0; i<9; i++) 
            fl_line(i*w()/8,h(),i*w()/8,h()-5);
        }
       }
       fl_color(FL_BLACK);
       fl_font(FL_TIMES, 10);
       fl_draw("pi/2", 3, (int)(ppi*0.5*yscale+ybase)-3);
       fl_draw("pi/2", 3, (int)(-ppi*0.5*yscale+ybase)+3);
       y0 = p[0]*yscale + ybase;
       x0 = 0.0;
       for (i=1; i<STEPS; i++) {
         double x1, y1;
         x1 = x0 + xscale;
         y1 = ybase + p[i]*yscale;
         if ((dy*(p[i]-p[i-1])<0) && (p[i]+p[i-1]<0.3));
         else fl_line((int)x0,h()-(int)y0,(int)x1,h()-(int)y1);
         //         if (p[i]*p[i-1]>=0 || p[i]*p[i-1]<9)
         //         fprintf(stderr,"%f %f %f %f\n", x0,y0,x1,y1);
         dy = y1-y0;
         x0 = x1; y0 = y1;
       }
    }
    else fprintf(stderr, "no data\n");
}

void Fl_ZGraph::draw()
{
    Fl_Window::draw();
//     double b0 = B0->value();
//     if (b0==0.0) {
//       valid_data = 0;
//       return;
//     }
    int size = h();
    if (w()<size) size = w();
    size -= 10;
    a0 = A0->value();
    a1 = A1->value();
    a2 = A2->value();
    b1 = B1->value();
    b2 = B2->value();
    double p0,p1,pi,z0,z1,zi;
    // ZEROS
    if (a2==0.0) {
      if (a1==0.0) {
        zi = z1 = z0 = 0.0;
      }
      else {
        z1 = z0 = -a1/a0;
        zi = 0.0;
      }
    }
    else {
      double disc=a1*a1-(4*a2*a0);
      if (disc<0.0) {
        zi = sqrt(-disc)/(a0+a0);
        z0 = z1 = -a1/(a0+a0);
      }
      else {
        zi = 0;
        z0=(sqrt(disc)-a1)/(a0+a0);
        z1=(-sqrt(disc)-a1)/(a0+a0);
      }
    }
    //    printf("Zeros: (%f,%f) and (%f,%f)\n", z0, zi, z1, -zi);
    // POLES
    if (b2==0.0) {
      if (b1==0.0) {
        pi = p1 = p0 = 0.0;
      }
      else {
        p1 = p0 = -b1;
        pi = 0.0;
      }
    }
    else {
      double disc=b1*b1-(4*b2);
      if (disc<0.0) {
        pi = sqrt(-disc)/2.0;
        p0=p1=(-b1)/2.0;

      }
      else {
        pi = 0;
        p0=(sqrt(disc)-b1)/2.0;
        p1=(-sqrt(disc)-b1)/2.0;
      }
    }
    //    printf("Poles: (%f,%f) and (%f,%f)\n", p0, pi, p1, pi);
    double sr1 = z0*z0+zi*zi;
    double sr2 = p0*p0+pi*pi;
    double sr3 = z1*z1+zi*zi;
    double sr4 = p1*p1+pi*pi;
    double radius = (sr1>sr2 ? sr1 : sr2);
    radius = (sr3>radius ? sr3 : radius);
    radius = (sr4>radius ? sr4 : radius);
    if (radius<1.0) radius = 1.0;
    radius = sqrt(radius);
    // scale so radius === size
    double scale = 0.5*size/radius;
    // Cannot get this to actually work!!!
    if (freq >= 0.0) {
      fl_color(FL_DARK_MAGENTA);
      fl_circle(w()/2+cos(TWOPI*freq)*scale,
                h()/2-sin(TWOPI*freq)*scale, 3); // Draw circle
      //fl_circle(w()/2+cos(TWOPI*freq)*scale,
      //          h()/2-sin(TWOPI*freq)*scale, 4); // Draw second circle
    }
    fl_color(FL_BLACK);
    fl_circle(w()/2, h()/2, scale); // Draw unit circle
    fl_line_style(FL_DOT);
    fl_line(0,h()/2,w(),h()/2);
    fl_line(w()/2,0,w()/2,h());
    fl_line_style(FL_SOLID);
    if (z0!=0.0 || z1!=0.0 || zi!=0.0) {
      fl_color(FL_RED);
      fl_circle(w()/2+z0*scale, h()/2+zi*scale, 5); // Draw zero
      fl_circle(w()/2+z1*scale, h()/2-zi*scale, 5);
    }
    if (p0!=0.0 || p1!=0.0 || pi!=0.0) {
      if (p0*p0+pi*pi>=1.0) {
        unstable = 1;
        fl_color(FL_DARK_YELLOW);
      }
      else {
        unstable = 0;
        fl_color(FL_DARK_BLUE);
      }
      fl_line((int)(w()/2.0+p0*scale+5.0), (int)(h()/2.0+pi*scale+5.0),
              (int)(w()/2.0+p0*scale-5.0), (int)(h()/2.0+pi*scale-5.0));
      fl_line((int)(w()/2.0+p0*scale+5.0), (int)(h()/2.0+pi*scale-5.0),
              (int)(w()/2.0+p0*scale-5.0), (int)(h()/2.0+pi*scale+5.0));
      
      if (p1*p1+pi*pi>=1.0) {
        unstable = 1;
        fl_color(FL_DARK_YELLOW);
      }
      else {
        unstable = 0;
        fl_color(FL_DARK_BLUE);
      }
      fl_line((int)(w()/2.0+p1*scale+5.0), (int)(h()/2.0-pi*scale+5.0),
              (int)(w()/2.0+p1*scale-5.0), (int)(h()/2.0-pi*scale-5.0));
      fl_line((int)(w()/2.0+p1*scale+5.0), (int)(h()/2.0-pi*scale-5.0),
              (int)(w()/2.0+p1*scale-5.0), (int)(h()/2.0-pi*scale+5.0));
    }
}

void Fl_TGraph::draw()
{
    Fl_Window::draw();
    double scale = 1.0;
    double base = 0.0;
    int i,j;
    for (i=0; i<TSTEPS-1; i++) {
      if (::y[i]>scale) scale = ::y[i];
      if (-::y[i]>scale) scale= -::y[i];
    }
    //    printf("scale = %f\n");
    base = scale;
    scale = scale * 2.1/h();
    fl_line_style(FL_SOLID);
    fl_color(FL_BLUE);
    for (i=0, j=0; (j<w()-5) && (i<TSTEPS-1); j += 3, i++)
      fl_line(j, h()/2, j, h()/2-(int)(::y[i]/scale));
    //    printf("(h=%d) %f %f %f %f\n&%f     %d %d %d %d\n",
    //           h(), ::y[2], ::y[3],::y[4], ::y[5],
    //           base, h()/2-(int)(::y[2]/scale),h()/2-(int)(::y[3]/scale),
    //           h()/2-(int)(::y[4]/scale),h()/2-(int)(::y[5]/scale));
}

void do_calc()
{
    if (changed) {
      int i;
//       double b0 = B0->value();
//       if (b0==0.0) {
//         valid_data = 0;
//         return;
//       }
      a0 = A0->value();
      a1 = A1->value();
      a2 = A2->value();
      b1 = B1->value();
      b2 = B2->value();
      if (logscale->value() == 0) 
        for (i=0; i<STEPS; i++) {
          double w = (double)(i)*ppi/(double(STEPS));
          f_p_response(w, &f[i], &p[i]);
        }
      else
        for (i=1; i<=STEPS; i++) {
          // space points logarithmetically
          double w = ppi*pow(2204.0,(double)(i)/(double)(STEPS))/2204.0;
         // printf("i=%d x=%f w=%f\n", i, (double)(i)/(double)(STEPS), w);
         // printf("i=%d x=%f f=%f\n", i, (double)(i)/(double)(STEPS),22050.0/M_PI  * w);
          f_p_response(w, &f[i-1], &p[i-1]);
        }

      x[0] = x[1] = y[0] = y[1] = 0.0; x[2] = 1.0;
      for (i=2; i<TSTEPS-1; i++) {
        y[i] = (a0*x[i]+a1*x[i-1]+a2*x[i-2]-b1*y[i-1]-b2*y[i-2]);
        if (y[i]>1000000.0) y[i] = 1000000.0; // Explosion
        else if (y[i]<-1000000.0) y[i] = -1000000.0;
        x[i+1] = 0.0;
      }
      valid_data = 1;
      changed = 0;
    }
}

void do_freq(Fl_Button*, void*)
{
    if (fgraph->visible()) fgraph->hide();
    else {
      do_calc();
      fgraph->position(620,150);
      fgraph->show();
    }
}

void do_phase(Fl_Button*, void*)
{
    if (pgraph->visible()) pgraph->hide();
    else {
      do_calc();
      pgraph->position(620,325);
      pgraph->show();
    }
}

void do_zplane(Fl_Button*, void*)
{
    if (zgraph->visible()) zgraph->hide();
    else {
      do_calc();
      zgraph->position(350,320);
      zgraph->show();
    }
}

void do_impulse(Fl_Button*, void*)
{
    if (tgraph->visible()) tgraph->hide();
    else {
      do_calc();
      tgraph->position(620,500);
      tgraph->show();
    }
}

static void do_options(Fl_Button*, void*)
{
    if (options->visible()) options->hide();
    else {
      options->position(413,600);
      options->show();
    }
}

static void do_sound(Fl_Button*, void*)
{
    if (sound->visible()) sound->hide();
    else {
      sound->position(14,330);
      sound->show();
    }
}

int main(int argc, char **argv)
{
    Fl_Window* www;
    int i;
    int j=0;
    int showf = 0, showp = 0, showz = 0, showi = 0, showl = 0, showd = 0;

    if (argc>1) {
      for (i=1; i<argc; i++)
        if (isalpha(argv[i][0])) {
          int k = 0;
          while (argv[i][k]!='\0') {
            switch (argv[i][k]) {
            case 'd': case 'D':
              showd = 1; break;
            case 'l': case 'L':
              showl = 1; break;
            case 'f': case 'F':
              showf = 1; break;
            case 'p': case 'P':
              showp = 1; break;
            case 'z': case 'Z':
              showz = 1; break;
            case 'i': case 'I':
              showi = 1; break;
            default:
              fprintf(stderr, "Unknown option %c\n", argv[i][k]);
            }
            k++;
          }
        }
        else {
          if (j>5) {
            fprintf(stderr, "Too many arguments\n");
            exit(1);
          }
          Default[j++] = atof(argv[i]);
          //          printf("Default[%d] = %f\n", j-1, Default[j-1]);
        }
    }
#ifndef JPFF    
    osc = new_oscil(44100.0);
#endif
    www    = make_window();
    fgraph = make_box1();
    pgraph = make_box2();
    zgraph = make_box3();
    tgraph = make_box4();
    options = make_options();
    sound = make_player();
    if (showl) logscale->value(1);
    if (showd) dbscale->value(1);
    do_calc();
    www->show();
    if (showf) fgraph->show();
    if (showp) pgraph->show();
    if (showz) zgraph->show();
    if (showi) tgraph->show();
    while (1) {                 // This way there is a possibility of update
      Fl::wait(0.15);           // while audio is playing
    }
}

#ifndef JPFF
OSCIL*   new_oscil(double srate)
{
	OSCIL* p_osc;

	p_osc = (OSCIL*) malloc(sizeof(OSCIL));
	if(p_osc==NULL)
		return NULL;
	p_osc->twopiovrsr = TWOPI / srate;
	p_osc->curfreq = 0.0;
	p_osc->curphase = 0.0;
	p_osc->incr = 0.0;
	return p_osc;
}



double sinetick(OSCIL* p_osc, double freq)
{
	double val;

	val = sin(p_osc->curphase);
	if(p_osc->curfreq != freq){
		p_osc->curfreq = freq;
		p_osc->incr = p_osc->twopiovrsr * freq;		
	}	
	p_osc->curphase += p_osc->incr;
	if(p_osc->curphase >= TWOPI) p_osc->curphase -= TWOPI;
    if(p_osc->curphase < 0.0) p_osc->curphase += TWOPI;
	return val;
}
#endif
