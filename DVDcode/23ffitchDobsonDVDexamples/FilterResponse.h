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

#ifndef Fresponse_h
#define Fresponse_h
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Value_Input.H>
extern Fl_Value_Input *A0;
extern Fl_Value_Input *A1;
extern Fl_Value_Input *A2;
extern Fl_Value_Input *B0;
extern Fl_Value_Input *B1;
extern Fl_Value_Input *B2;
#include <FL/Fl_Button.H>
extern void do_calc(void);
#include <FL/Fl_Box.H>
extern void do_phase(Fl_Button*, void*);
extern void do_freq(Fl_Button*, void*);
extern void do_zplane(Fl_Button*, void*);
extern void do_impulse(Fl_Button*, void*);
#include <FL/Fl_Check_Button.H>
extern Fl_Check_Button *dbscale;
#include <FL/Fl_Output.H>
Fl_Window* make_window();
//Fl_FGraph* make_box1();
//Fl_PGraph* make_box2();
//Fl_ZGraph* make_box3();
#include <FL/Fl_Counter.H>
extern Fl_Counter *resolution;
extern Fl_Counter *mindb;
extern Fl_Window *Player;
#include <FL/Fl_File_Input.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Round_Button.H>
Fl_Window* make_player();
#endif
