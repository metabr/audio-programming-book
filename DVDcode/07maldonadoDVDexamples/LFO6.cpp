#include "LFO6.h"

LFO6 *lfoApp;
const int FPB = 256;
const U_LONG ASIGNAL::blockLen = FPB;

inline void LFO6::synthesis_callback(float *outputBuffer, U_LONG framesPerBuffer) 
{   
    double  outLFO1 = lfo1->tic(ampLFO1, freqLFO1);
    double  outLFO2 = lfo2->tic(ampLFO2, freqLFO2);
    double  outLFO3 = lfo3->tic(ampLFO3, freqLFO3);
    ASIGNAL outLFO4 = lfo4->perfBlock(1, freqLFO4);
    ASIGNAL outMod = modulator->perfBlock(ampModulator * (1 + ampLFO2 + outLFO2), freqModulator * pow(2, outLFO1));
    ASIGNAL outCar = carrier->perfBlock(outLFO4, (freqCarrier + outMod) * pow(2, outLFO3));
    asigStereoOut(outputBuffer, outCar);
}

// audio callback function adapter
int LFO6_callback( const void *inBuf, void *outBuf, U_LONG framesPerBuffer, 
        const PaStreamCallbackTimeInfo* tInfo, PaStreamCallbackFlags flags, void *userData )
{   
    lfoApp->synthesis_callback((float *) outBuf, framesPerBuffer);
    return paContinue;
}

LFO6::LFO6()  // constructor
{
    SAMPLING_RATE = 44100;
    FRAMES_PER_BUFFER = FPB;
    CONTROL_RATE = SAMPLING_RATE / FRAMES_PER_BUFFER;
    
    const double frqMin = .02, frqMax = 20;
    
    freqLFO1 = freqLFO2 = freqLFO3 = freqLFO4 = 5;
    ampLFO1 = ampLFO2 = ampLFO3 = ampModulator = 0;
    freqCarrier = freqModulator = 100;
    logMin = log(frqMin);
    logMax = log(frqMax);
    ampMax = 4;
    audio_callback = LFO6_callback;
    show();

    table = new TABLE[4];
    lfo1 = new OSCILIextTABLE(table[0], CONTROL_RATE);
    lfo2 = new OSCILIextTABLE(table[0], CONTROL_RATE);
    lfo3 = new OSCILIextTABLE(table[0], CONTROL_RATE);
    lfo4 = new OSCILIextTABLE(table[0], SAMPLING_RATE); 
    carrier   = new OSCILIextTABLE(table[0], SAMPLING_RATE);
    modulator = new OSCILIextTABLE(table[0], SAMPLING_RATE);

    for (int j=0; j < 4; j++) redrawShape(tableShape[j], &table[j]);
}

void computeSumOfPartials(float *table, long tablen, void* partialData)  // fills target table with
{                        //  a sum of partials of having any shape, even different in each partial
    static TABLE shapes[6] = { TABLE(TABLE_LEN, Sine), TABLE(TABLE_LEN, Triangle), // table objects
                              TABLE( TABLE_LEN, UpRamp), TABLE(TABLE_LEN, DownRamp), // conataining
                              TABLE( TABLE_LEN, Square) };           // the various starting shapes 
    PARTIALS_DATA *pdata = (PARTIALS_DATA *) partialData;
    long i, k;
    float* tb5 = shapes[5].getTable();              // get the custom shape (index 5) and
    for (i = 0; i < tablen; i++) tb5[i] = table[i]; // fill it with current table content
    for(i=0; i<tablen; i++) table[i] = 0; // clear target table array
    double max=0;
    for (k=0; k < tablen; k++) {                    // traverse all table array elements
        for (i=0; i < pdata->numOfPartials; i++) {  // traverse all partials
            long index = (long) (pdata->numHarm[i] * k + pdata->phase[i] * tablen); // get table index
            while (index >= tablen) index -= tablen;        // wrap it around if exceeds table length
            table[k] += (float) (shapes[pdata->shape[i]].getTable()[index] * pdata->levelHarm[i]);
        }
        if (fabs(table[k]) > max) max = fabs(table[k]);      // update maximum value
    }
    double norm_factor = 1/max; // get the normalizing factor
    for (k=0; k < tablen; k++)  // normalize wave
        table[k] *= (float) norm_factor;
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
    lfoApp = new LFO6;
    Fl::run();
    delete lfoApp;
    return 0;
}
