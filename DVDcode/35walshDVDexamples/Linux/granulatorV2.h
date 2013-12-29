/*********************** granulatorV2.h **************************/
#ifndef MYFRAME_H
#define MYFRAME_H
#include "wx/wx.h"
#include "csound.hpp"
#include "csPerfThread.hpp" 
#include <iostream>

#ifdef __WXMAC__
#include <mach-o/dyld.h>
#endif

using namespace std;

class MyApp: public wxApp{
    virtual bool OnInit();
};




//sliderBox class
class mySliderBox : public wxPanel{
public:
wxSlider* slider;
wxTextCtrl* textCtrl;
int sliderRange;
mySliderBox(wxWindow* parent, wxWindowID id, wxString caption, float range, const wxPoint &pos, const wxSize &size, wxString name) 
: wxPanel(parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, name)
	{
	wxStaticBox* staticBox = new wxStaticBox(this, -1, caption);
	wxStaticBoxSizer* sizer = new wxStaticBoxSizer(staticBox, wxHORIZONTAL);
	sliderRange = range;
    slider = new wxSlider(this, id, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, name);  	
	sizer->Add( slider, 4, 0, 0 );
	textCtrl = new wxTextCtrl(this, -1, wxT("0.0"));
	sizer->Add(textCtrl, 1, 0, 0); 
	SetSizer(sizer);
	Layout();
	}	
};

//output dialog class
#define outputDlg_STYLE wxRESIZE_BORDER | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX

class outputDlg : public wxDialog
{
public:
wxTextCtrl*	textCtrl;
outputDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Csound Message Output"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style =outputDlg_STYLE)
  : wxDialog(parent, id, title, pos, size, outputDlg_STYLE)
    {
    wxPanel* panel = new wxPanel(this, -1, wxDefaultPosition, this->GetSize());
    textCtrl = new wxTextCtrl(panel, -1, wxT(""), wxPoint(1,2), wxSize(panel->GetSize()), wxVSCROLL | wxTE_MULTILINE | wxTE_READONLY, wxDefaultValidator, wxT("WxMemo1"));
    textCtrl->SetMaxLength(0);
    wxFont textFont = wxFont(8, wxMODERN, wxNORMAL, wxBOLD);
    textCtrl->SetFont(textFont);
    wxString version;
    version << wxT("Csound Version:") << (float)csoundGetVersion()/1000.f;
    textCtrl->AppendText(version);
    textCtrl->SetFocus();
    textCtrl->SetInsertionPointEnd();
    textCtrl->SetForegroundColour(*wxGREEN);
    textCtrl->SetBackgroundColour(*wxBLACK);
    }
//private:
};

wxString str;
void csoundMessageCall(CSOUND* csound, int attr, const char* format, va_list valist);
    
class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	wxPanel* panel;
	wxSlider* slider;
	wxMenu *menu;
	mySliderBox* freqSliderBox;
	mySliderBox* densSliderBox;
	mySliderBox* pitchOffsetSliderBox;
	mySliderBox* ampOffsetSliderBox;
	mySliderBox* grainDurationSliderBox;	
	wxSizer* mysizer;
	wxTimer* timer;
	wxString comp;
	
	Csound* csound;
  	CsoundPerformanceThread* csThread;
	outputDlg* csoundOutput;
	
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void ScrollEvent(wxScrollEvent& event);
    void StartStop(wxCommandEvent &event);
    void UpdateMessages(wxCommandEvent &event){}
    void ViewOutput(wxCommandEvent &event){
	csoundOutput->Show(true);    
    }
    void TimerFunc(wxTimerEvent& event){
      if(comp!=str){
	comp = str; 
        csoundOutput->textCtrl->SetValue(str);
        }
    }



    wxString float2wxStr(float value){
	wxString mystring = wxString::Format(wxT("%2.2f"), value);
	return mystring;    
    }
    
    DECLARE_EVENT_TABLE()
      public:    
	enum{
	    ID_Quit = 100,
	    ID_About = 200,
	    ID_SAMPLE = 300,
	    ID_STARTSTOP = 400,
	    ID_SCROLL = 500,
	    ID_VIEWOUTPUT = 600,
	    ID_TIMER = 700
	};
};

struct CsoundUserData{
  CSOUND *csound; // instance of csound
  MyFrame *myFrame; //pass main application to check widgets
};

#endif
