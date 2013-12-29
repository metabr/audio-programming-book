#include "LFO3.h"

LFO3 *lfoApp;

inline void LFO3::synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer) 
{       // the audio callback implementation, belonging to the LFO1 class
    
    double outLFO = lfo->tic(ampLFO, freqLFO);// get the ouput of the LFO
    carrier->input(1, freqCarrier + outLFO);  // get the actual frequency of the carrier

    float *out = outputBuffer;
    for( unsigned long i=0; i < framesPerBuffer; i++) { // the audio engine loop
        float outCarrier = (float) carrier->tic();      // get the output of the carrier
        *out++ = outCarrier;        // stream to the stereo ouptut (left channel)
        *out++ = outCarrier;        // right channel
    }
}

// audio callback function adapter
int LFO3_callback( const void *inBuf, void *outBuf, U_LONG framesPerBuffer, 
        const PaStreamCallbackTimeInfo* tInfo, PaStreamCallbackFlags flags, void *userData )
{   lfoApp->synthesis_callback((float *) outBuf, framesPerBuffer);
    return paContinue;
}

LFO3::LFO3()  // constructor
{
    SAMPLING_RATE = 44100;
    FRAMES_PER_BUFFER = 256;
    CONTROL_RATE = SAMPLING_RATE / FRAMES_PER_BUFFER;

    const double frqMin = .1, frqMax = 100;
    
    freqLFO = 5;
    logMin = log(frqMin);
    logMax = log(frqMax);
    ampMax = 300;
    ampLFO = 0;
    audio_callback = LFO3_callback;
    show();
    
    freqCarrier = 200;
    lfo = new OSCILI(Sine, CONTROL_RATE);
    carrier = new OSCILI(Sine, SAMPLING_RATE);

    carrier_radio_callback(Sine);  // display carrier waveform
    LFO_radio_callback(Sine);      // display LFO waveform
}

void computeSumOfSines(float *table, long tablen, void* sinData)
{
    PARTIALS_DATA *sdata = (PARTIALS_DATA *) sinData;
    long i, k;
    for(i=0; i<tablen; i++) table[i] = 0; // clear table
    double max=0;
    for (k=0; k < tablen; k++) {
        
        for (i=0; i < sdata->numOfPartials; i++) {
            table[k] += (float) (sin(TWO_PI * sdata->numHarm[i] * k/tablen) * sdata->levelHarm[i]);
        }
        if (fabs(table[k]) > max) max = fabs(table[k]);
    }
    
    double norm_factor = 1/max; // normalize wave
    for (k=0; k < tablen; k++) 
        table[k] *= (float) norm_factor;
}

void drawWaveform(Fl_DrawShape * o) // draw the waveform to current widget
{
    int X = o->x()+1;           // get the X position of the widget
    int Y = o->y()+2;           // get the Y position of the widget
    int width  = o->w()-2;      // get the width of the widget 
    int height = o->h()-4;      // get the height of the widget
    float *table = o->table;    // get the table pointer
    long tablen = o->tablen;    // get the table length
    int old_x = 0;           
    fl_color(50,64,50);         // set color of the middle horiz. line
    fl_line_style(FL_DOT);      // set a dotted line style 
    fl_line(X,Y+height/2, X + width, Y+height/2);    // draw the middle horiz. line
    fl_color(64,0,0);           // set line color of the waveform
    fl_line_style(FL_SOLID);    // set a solid line style
    fl_begin_line();            // begin the drawing session of the waveform
    for (long i = 0; i < tablen; i++) { // draw sample by sample
        int x = (int) (i * ((float) width / (float) tablen)); // adapt the x coord 
        int y = (int) (height - (table[i] + 1) * (float) height/2); // adapt the y coord
        if(x != old_x ) {       // skip redundant points
            fl_vertex(X+x, Y+y);// draw a line up to this point
            old_x = x;          // update to current index
        }
    }
    fl_end_line();              // end the waveform drawing session
}

int main()
{
    lfoApp = new LFO3;
    Fl::run();
    delete lfoApp;
    return 0;
}
