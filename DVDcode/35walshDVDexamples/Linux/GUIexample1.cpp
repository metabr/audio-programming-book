/******************** GUIexample1.cpp **************************/
#include "GUIexample1.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
   MyFrame *frame = new MyFrame(wxT("GUI Example No.1"), wxPoint(50,50), wxSize(350,240));
   frame->Show(TRUE);
   return TRUE;
} 

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
    EVT_MENU(ID_STARTSTOP, MyFrame::StartStop)
    EVT_BUTTON(ID_STARTMEL1, MyFrame::StartMelody1)
    EVT_BUTTON(ID_STARTMEL2, MyFrame::StartMelody2)
    EVT_BUTTON(ID_STARTMEL3, MyFrame::StartMelody3)
    EVT_BUTTON(ID_STARTMEL4, MyFrame::StartMelody4)
    EVT_BUTTON(ID_STARTMEL5, MyFrame::StartMelody5)
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
    wxFrame::SetMenuBar( menuBar ); 
    panel = new wxPanel(this, -1, wxPoint(0, 0), wxFrame::GetSize());
    button1 = new wxButton(panel, ID_STARTMEL1, wxT("Melody 1"), wxPoint(40, 40), wxSize(80, 40));
    button2 = new wxButton(panel, ID_STARTMEL2, wxT("Melody 2"), wxPoint(120,40), wxSize(80, 40));
    button3 = new wxButton(panel, ID_STARTMEL3, wxT("Melody 3"), wxPoint(200,40), wxSize(80, 40));
    button4 = new wxButton(panel, ID_STARTMEL4, wxT("Melody 4"), wxPoint(40,  100), wxSize(80, 40));
    button5 = new wxButton(panel, ID_STARTMEL5, wxT("Melody 5"), wxPoint(120, 100), wxSize(80, 40));
    
    csound = new Csound;
    csound->PreCompile();

int result = csound->Compile("example9.csd");
    if(result)wxMessageBox(wxT("error compiling"));
	csThread = new CsoundPerformanceThread(csound);
}

void MyFrame::StartStop(wxCommandEvent& event){
if(!strcmp(menu->GetLabel(800).mb_str(), "Start")){
	menu->SetLabel(800, wxT("Stop"));
	csThread->Play();
	csound->SetChannel("loop", 1);
	}
 else if(!strcmp(menu->GetLabel(800).mb_str(), "Stop")){
	menu->SetLabel(800, wxT("Start"));
	csThread->Pause();
	}
}



void MyFrame::OnQuit(wxCommandEvent& event){
 csThread->Stop();
 csThread->Join();
 csound->Cleanup();
 delete csThread;
 delete csound; 
 Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& event){
    wxMessageBox(wxT("GUI Example 1"));
}

void MyFrame::StartMelody1(wxCommandEvent& event){
csound->SetChannel("loop", 1);	
}

void MyFrame::StartMelody2(wxCommandEvent& event){
csound->SetChannel("loop", 2);	
}

void MyFrame::StartMelody3(wxCommandEvent& event){
csound->SetChannel("loop", 3);	
}

void MyFrame::StartMelody4(wxCommandEvent& event){
csound->SetChannel("loop", 4);	
}

void MyFrame::StartMelody5(wxCommandEvent& event){
csound->SetChannel("loop", 5);	
}
