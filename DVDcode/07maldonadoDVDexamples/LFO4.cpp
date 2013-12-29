#include "LFO4.h"

LFO4 *lfoApp;

inline void LFO4::synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer) 
{       // the audio callback implementation, belonging to the LFO1 class

    double outLFO = lfo->tic(ampLFO, freqLFO);       // get the ouput of the LFO
    carrier->input(1, freqCarrier * pow(2,outLFO));  // get the actual frequency of the carrier

    float *out = outputBuffer;
    for( unsigned long i=0; i < framesPerBuffer; i++) { // the audio engine loop
        float outCarrier = (float) carrier->tic();      // get the output of the carrier
        *out++ = outCarrier;        // stream to the stereo ouptut (left channel)
        *out++ = outCarrier;        // right channel
    }
}

// audio callback function adapter
int LFO4_callback( const void *inBuf, void *outBuf, U_LONG framesPerBuffer, 
        const PaStreamCallbackTimeInfo* tInfo, PaStreamCallbackFlags flags, void *userData )
{   lfoApp->synthesis_callback((float *) outBuf, framesPerBuffer);
    return paContinue;
}

LFO4::LFO4()  // constructor
{
    SAMPLING_RATE = 44100;
    FRAMES_PER_BUFFER = 256;
    CONTROL_RATE = SAMPLING_RATE / FRAMES_PER_BUFFER;

    const double frqMin = .02, frqMax = 20;

    freqLFO = 5;
    logMin = log(frqMin);
    logMax = log(frqMax);
    ampMax = 4; //new amp for exponential
    ampLFO = 0;
    audio_callback = LFO4_callback;
    show();

    freqCarrier = 200;
    lfo = new OSCILI(Sine, CONTROL_RATE);
    carrier = new OSCILI(Sine, SAMPLING_RATE);
    
    carrier_radio_callback(0);
    LFO_radio_callback(0);
}

void computeSumOfPartials(float *table, long tablen, void* partialData)     // fills target table
{                                                   // with a sum of partials of having any shape
    PARTIALS_DATA *pdata = (PARTIALS_DATA *) partialData; 
    long i, k;
    float *newTable= new float[tablen];             // create a temporary table
    for(i=0; i<tablen; i++) newTable[i] = table[i]; // save current oscillator table contents  
    for(i=0; i<tablen; i++) table[i] = 0;           // clear oscillator table
    double max=0;
    for (k=0; k < tablen; k++) {                    // traverse all table elements
        for (i=0; i < pdata->numOfPartials; i++) {  // traverse all partials
            long index = (long) pdata->numHarm[i] * k;  // get the appropriate table index 
            while (index >= tablen) index -= tablen;    // wrap it around if exceeds table length
            table[k] += (float) (newTable[index] * pdata->levelHarm[i]); // accumulate sample values
        }
        if (fabs(table[k]) > max) max = fabs(table[k]); // update maximum value
    }
    double norm_factor = 1/max; // get the normalizing factor
    for (k=0; k < tablen; k++)  // normalize wave
        table[k] *= (float) norm_factor; 
    delete [] newTable;         // delete the temporary table
}

void drawWaveform(Fl_DrawShape * o) 
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
    lfoApp = new LFO4;
    Fl::run();
    delete lfoApp;
    return 0;
}
