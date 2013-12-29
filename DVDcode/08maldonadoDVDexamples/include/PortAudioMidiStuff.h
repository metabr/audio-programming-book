#ifndef __pamstuff__
#define __pamstuff__
#include <FL/fl_ask.H>  // an FLTK header for fl_message()
#include "PortAudioStuff.h"
#include "PortMidiStuff.h"

typedef unsigned long U_LONG; // abbreviates the digitation of unsigned long

class PAMstuff : public PAstuff, public PMstuff // multiple inheritance
{       
  protected:
    void make_menu() { // makes a menu bar made up of both PAstuff and PMstuff
        PAstuff::make_menu();           // call corresponding method of PAstuff
        PMstuff::make_menu();           // call corresponding method of PMstuff 
        MidiMenuBar->position(236,0);   // reposition the menu bar of PMstuff
        PMstuff::menuExit->hide();      // hide the "exit" menu of PMstuff
        PMstuff::menuStartMidi->hide(); // hide the "start" menu of PMstuff
        PMstuff::menuStopMidi->hide();  // hide the "stop" menu of PMstuff
    }
    virtual void play() {  // this overrides PAstuff play method
        if (PAstuff::isOutputDeviceChosen // if both audio and MIDI devices have been chosen
            && (PMstuff::isOutputDeviceChosen || PMstuff::isInputDeviceChosen)) {
            PMstuff::play();  // start playing of both MIDI
            PAstuff::play();  // and audio
        }
        else      // if user forgt to choose audio and/or MIDI devices
            fl_message("You have to choose both audio and midi I/O ports prior to play!");
    }
    virtual void stop() {    // overrides the PAstuff stop method
            PMstuff::stop(); // call corresponding method of PMstuff    
            PAstuff::stop(); // call corresponding method of PAstuff    
    }
  public:
    PAMstuff(int numOutChans=2, int numInChans=0, // constructor
             bool midiInEnabled=true, bool midiOutEnabled=false) 
     : PAstuff(numOutChans, numInChans), PMstuff(midiInEnabled, midiOutEnabled)
    { }
};

#endif