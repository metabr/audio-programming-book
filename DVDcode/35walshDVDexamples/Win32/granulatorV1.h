/*********************** granulatorV1.h **************************/
#ifndef MYFRAME_H
#define MYFRAME_H
#include "wx/wx.h"
#include "csound.hpp"
#include "csPerfThread.hpp"
#include <iostream>

using namespace std;

class MyApp: public wxApp{
    virtual bool OnInit();
};

class MyFrame: public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
	
    void FreqScrollEvent(wxScrollEvent& event);
	void DensScrollEvent(wxScrollEvent& event);
	void AmpOffScrollEvent(wxScrollEvent& event);
	void PitchOffScrollEvent(wxScrollEvent& event);
	void GrainDurScrollEvent(wxScrollEvent& event);
	
    void StartStop(wxCommandEvent &event);

    wxMenu *menu;
	wxMenuBar *menuBar;
	wxSlider* freqSlider;
	wxTextCtrl* freqTextCtrl;
	wxSlider* densSlider;
	wxTextCtrl* densTextCtrl;
	wxSlider* ampoffSlider;
	wxTextCtrl* ampoffTextCtrl;
	wxSlider* pitchoffSlider;
	wxTextCtrl* pitchoffTextCtrl;
	wxSlider* graindurSlider;
	wxTextCtrl* graindurTextCtrl;	
  	
  	Csound* csound;
  	CsoundPerformanceThread* csThread;
  	
  	float freqRange, densRange, ampoffRange, pitchoffRange, graindurRange;
  	
  	DECLARE_EVENT_TABLE()

enum{
    ID_Quit = 100,
    ID_About = 200,
    ID_SAMPLE = 300,
    ID_STARTSTOP = 400,
    ID_SCROLL1 = 500,
    ID_SCROLL2 = 600,
    ID_SCROLL3 = 700,
    ID_SCROLL4 = 800,
    ID_SCROLL5 = 900
};
    
};
#endif
