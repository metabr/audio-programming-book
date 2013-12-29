// generated by Fast Light User Interface Designer (fluid) version 1.0104

#ifndef LFO1gui_h
#define LFO1gui_h
#include <FL/Fl.H>
#include "PortAudioStuff.h"
#include <FL/Fl_Window.H>
#include <FL/Fl_Positioner.H>
#include <FL/Fl_Value_Input.H>

class LFO1gui : public PAstuff {
public:
  LFO1gui();
  Fl_Window *mainWindow;
  Fl_Positioner *joy;
private:
  inline void cb_joy_i(Fl_Positioner*, void*);
  static void cb_joy(Fl_Positioner*, void*);
public:
  Fl_Value_Input *CarrierFreq;
  void show();
  void bye();
  virtual void joy_callback();
};
#endif
