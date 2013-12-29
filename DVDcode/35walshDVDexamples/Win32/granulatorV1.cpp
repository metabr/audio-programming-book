/******************** granulatorV1.cpp **************************/
#include "granulatorV1.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit(){
   MyFrame *frame = new MyFrame(wxT("Granulator v.1"), wxPoint(50,50), wxSize(200,400));
   frame->Show(TRUE);
   return TRUE;
} 

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
    EVT_MENU(ID_STARTSTOP, MyFrame::StartStop)
    EVT_COMMAND_SCROLL(ID_SCROLL1, MyFrame::FreqScrollEvent)
    EVT_COMMAND_SCROLL(ID_SCROLL2, MyFrame::DensScrollEvent)
    EVT_COMMAND_SCROLL(ID_SCROLL3, MyFrame::AmpOffScrollEvent)
    EVT_COMMAND_SCROLL(ID_SCROLL4, MyFrame::PitchOffScrollEvent)
    EVT_COMMAND_SCROLL(ID_SCROLL5, MyFrame::GrainDurScrollEvent)
END_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
	menu = new wxMenu;
	menu->Append( ID_STARTSTOP, wxT("Start"));
	menu->Append( ID_About, wxT("&About..."));
	menu->Append( ID_Quit, wxT("E&xit"));
	menuBar = new wxMenuBar;
	menuBar->Append( menu, wxT("&File"));
	
	freqRange = 2;
	densRange = 100;
	ampoffRange = 2;
	pitchoffRange = 100;
	graindurRange = 1;
	
	wxFrame::SetMenuBar( menuBar );
	wxPanel* panel = new wxPanel(this, -1, wxPoint(0, 0), wxFrame::GetSize());
    
		wxStaticBox* staticBox1 = new wxStaticBox(panel, -1, wxT("Frequency"), wxPoint(10, 10), wxSize(150, 50)); 
	freqSlider = new wxSlider(panel, ID_SCROLL1, 0, 0, 100, wxPoint(15, 25), wxSize(100, 30), wxSL_HORIZONTAL, wxDefaultValidator, wxT("freq")); 	 	
	freqTextCtrl = new wxTextCtrl(panel, -1, wxT("0.0"), wxPoint(115, 30), wxSize(35, 20), 0, wxDefaultValidator);

	    wxStaticBox* staticBox2 = new wxStaticBox(panel, -1, wxT("Density"), wxPoint(10, 70), wxSize(150, 50)); 
	densSlider = new wxSlider(panel, ID_SCROLL2, 0, 0, 100, wxPoint(15, 85), wxSize(100, 30), wxSL_HORIZONTAL, wxDefaultValidator, wxT("dens")); 	 	
	densTextCtrl = new wxTextCtrl(panel, -1, wxT("0.0"), wxPoint(115, 90), wxSize(35, 20), 0, wxDefaultValidator);

	    wxStaticBox* staticBox3 = new wxStaticBox(panel, -1, wxT("Amplitude Offset"), wxPoint(10, 130), wxSize(150, 50)); 
	ampoffSlider = new wxSlider(panel, ID_SCROLL3, 0, 0, 100, wxPoint(15, 145), wxSize(100, 30), wxSL_HORIZONTAL, wxDefaultValidator, wxT("ampoff")); 	 	
	ampoffTextCtrl = new wxTextCtrl(panel, -1, wxT("0.0"), wxPoint(115, 150), wxSize(35, 20), 0, wxDefaultValidator);

	    wxStaticBox* staticBox4 = new wxStaticBox(panel, -1, wxT("Pitch Offset"), wxPoint(10, 190), wxSize(150, 50)); 
	pitchoffSlider = new wxSlider(panel, ID_SCROLL4, 0, 0, 100, wxPoint(15, 205), wxSize(100, 30), wxSL_HORIZONTAL, wxDefaultValidator, wxT("pitchoff")); 	 	
	pitchoffTextCtrl = new wxTextCtrl(panel, -1, wxT("0.0"), wxPoint(115, 210), wxSize(35, 20), 0, wxDefaultValidator);

	    wxStaticBox* staticBox5 = new wxStaticBox(panel, -1, wxT("Grain Duration"), wxPoint(10, 250), wxSize(150, 50)); 
	graindurSlider = new wxSlider(panel, ID_SCROLL5, 0, 0, 100, wxPoint(15, 265), wxSize(100, 30), wxSL_HORIZONTAL, wxDefaultValidator, wxT("graindur")); 	 	
	graindurTextCtrl = new wxTextCtrl(panel, -1, wxT("0.0"), wxPoint(115, 270), wxSize(35, 20), 0, wxDefaultValidator);

    csound = new Csound;
    csound->PreCompile();
	int result = csound->Compile("granny.csd");
	if(result)wxMessageBox(wxT("error compiling"));
    csThread = new CsoundPerformanceThread(csound);	
    csound->SetChannel("freq", 1);
    csound->SetChannel("dens", 10);
    csound->SetChannel("ampoff", 1);
    csound->SetChannel("pitchoff", 1);
    csound->SetChannel("graindur", 0.5);
		
}

void MyFrame::OnQuit(wxCommandEvent& event){
 csThread->Stop();
 csThread->Join();
 csound->Cleanup();
 //csound->Destroy();
 delete csThread;
 delete csound; 
 Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& event){
    wxMessageBox(wxT("Granular Synthesiser v.1"));
}

void MyFrame::StartStop(wxCommandEvent& event){
if(!strcmp(menu->GetLabel(400).mb_str(), "Start")){
	menu->SetLabel(400, wxT("Stop"));
	csThread->Play();
	}
 else if(!strcmp(menu->GetLabel(400).mb_str(), "Stop")){
	menu->SetLabel(400, wxT("Start"));
	csThread->Pause();
	}
}

void MyFrame::FreqScrollEvent(wxScrollEvent& event)
{
csound->SetChannel("freq", (freqSlider->GetValue()/100.f)*freqRange);
wxString mystring = wxString::Format(wxT("%2.2f"), (freqSlider->GetValue()/100.f)*freqRange);
freqTextCtrl->ChangeValue(mystring);
}

void MyFrame::DensScrollEvent(wxScrollEvent& event)
{
csound->SetChannel("dens", (densSlider->GetValue()/100.f)*densRange);
wxString mystring = wxString::Format(wxT("%2.2f"), (densSlider->GetValue()/100.f)*densRange);
densTextCtrl->ChangeValue(mystring);
}

void MyFrame::AmpOffScrollEvent(wxScrollEvent& event)
{
csound->SetChannel("ampoff", (ampoffSlider->GetValue()/100.f)*ampoffRange);
wxString mystring = wxString::Format(wxT("%2.2f"), (ampoffSlider->GetValue()/100.f)*ampoffRange);
ampoffTextCtrl->ChangeValue(mystring);
}

void MyFrame::PitchOffScrollEvent(wxScrollEvent& event)
{
csound->SetChannel("pitchoff", (pitchoffSlider->GetValue()/100.f)*pitchoffRange);
wxString mystring = wxString::Format(wxT("%2.2f"), (pitchoffSlider->GetValue()/100.f)*pitchoffRange);
pitchoffTextCtrl->ChangeValue(mystring);
}

void MyFrame::GrainDurScrollEvent(wxScrollEvent& event)
{
csound->SetChannel("grainddur", (graindurSlider->GetValue()/100.f)*graindurRange);
wxString mystring = wxString::Format(wxT("%2.2f"), (graindurSlider->GetValue()/100.f)*graindurRange);
graindurTextCtrl->ChangeValue(mystring);

}
