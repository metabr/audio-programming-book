/*********************** GUIexample1.h **************************/
#ifndef MYFRAME_H
#define MYFRAME_H
#include "wx/wx.h"
#include "csound.hpp"
#include "csPerfThread.hpp"

#ifdef __WXMAC__
#include <mach-o/dyld.h>
#endif

class MyApp: public wxApp
{
    virtual bool OnInit();
};


class MyFrame: public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    //command to start and stop Csound
    void StartStop(wxCommandEvent &event);    
    //our sample button event handler
    void StartMelody1(wxCommandEvent &event);
    void StartMelody2(wxCommandEvent &event);
    void StartMelody3(wxCommandEvent &event);
    void StartMelody4(wxCommandEvent &event);
    void StartMelody5(wxCommandEvent &event);
    
   	wxMenu *menu;   	  
   	wxMenuBar *menuBar;
	//out 5 buttons
   	wxButton *button1, *button2, *button3, *button4, *button5;
	//a panel widget to place our buttons on
  	wxPanel* panel;
  	Csound* csound;
  	CsoundPerformanceThread* csThread;
  	
enum{
    ID_Quit = 100,
    ID_About = 200,
    ID_STARTMEL1 = 300,
    ID_STARTMEL2 = 400,
    ID_STARTMEL3 = 500,
    ID_STARTMEL4 = 600,
    ID_STARTMEL5 = 700,
    ID_STARTSTOP = 800
};

DECLARE_EVENT_TABLE()
    
};
#endif
