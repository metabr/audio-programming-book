/******************** granulatorV2.cpp **************************/
#include "granulatorV2.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit(){
   MyFrame *frame = new MyFrame( wxT("Granulator v.2"), wxPoint(50,50), wxSize(250,350));
   frame->Show(TRUE);
   return TRUE;
} 

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
    EVT_COMMAND_SCROLL(ID_SCROLL, MyFrame::ScrollEvent)
    EVT_MENU(ID_STARTSTOP, MyFrame::StartStop)
    EVT_MENU(ID_VIEWOUTPUT, MyFrame::ViewOutput)
EVT_TIMER(ID_TIMER,MyFrame::TimerFunc)

END_EVENT_TABLE()


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    menu = new wxMenu;
    menu->Append( ID_STARTSTOP, wxT("Start"));
    menu->Append( ID_VIEWOUTPUT, wxT("View Csound Output"));
    menu->Append( ID_About, wxT("&About..." ));
    menu->Append( ID_Quit, wxT("E&xit" ));
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menu, wxT("&File"));
    wxFrame::SetMenuBar( menuBar );    
    csoundOutput = new outputDlg(this, -1, wxT("Csound Output"), wxPoint(this->GetPosition().x, this->GetPosition().y+this->GetSize().y), wxSize(550, 300));
    csoundOutput->Show(false);
    
    wxBoxSizer* sizer = new wxBoxSizer( wxVERTICAL );    
    panel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize);    
    freqSliderBox = new mySliderBox(panel, ID_SCROLL, wxT("Frequency"), 2, wxDefaultPosition, wxDefaultSize, wxT("freq"));
    sizer->Add( freqSliderBox, 1, wxEXPAND | wxALL, 5 );
    densSliderBox = new mySliderBox(panel, ID_SCROLL, wxT("Density"), 100, wxDefaultPosition, wxDefaultSize, wxT("dens"));
    sizer->Add( densSliderBox, 1, wxEXPAND | wxALL, 5 );	
    ampOffsetSliderBox = new mySliderBox(panel, ID_SCROLL, wxT("Amplitude Offset"), 2, wxDefaultPosition, wxDefaultSize, wxT("ampoff"));
    sizer->Add( ampOffsetSliderBox, 1, wxEXPAND | wxALL, 5 );	    
    pitchOffsetSliderBox = new mySliderBox(panel, ID_SCROLL, wxT("Pitch Offset"), 2, wxDefaultPosition, wxDefaultSize, wxT("pitchoff"));
    sizer->Add( pitchOffsetSliderBox, 1, wxEXPAND | wxALL, 5 );	
    grainDurationSliderBox = new mySliderBox(panel, ID_SCROLL, wxT("Grain Duration"), 1, wxDefaultPosition, wxDefaultSize, wxT("graindur"));
    sizer->Add( grainDurationSliderBox, 1, wxEXPAND | wxALL, 5 );				
    panel->SetSizer(sizer);
    panel->Layout();   	
	
    csound = new Csound;
    csound->PreCompile();
    timer = new wxTimer(this, ID_TIMER);
    timer->Stop();
    

#ifdef __WXMAC__
/* on OSX we can place our csd inside the bundle rather than simple
having it in the same folder as appliction. If you right-click the
GranulatorV2 bundle you can click on 'show contents'. You will see a
csd file in their called GranulatorV2.csd. In order to play this file you
must suipply the correct path to Csound. The path is found using teh code present
below */ 
char buf[1024];
uint32_t buflen = 1024;
_NSGetExecutablePath(buf, &buflen);
wxString str(buf, wxConvUTF8);
str.Append(wxT(".csd"));
char cstring[1024];
strcpy(cstring, (const char*)str.mb_str(wxConvUTF8));
int result = csound->Compile(cstring);
#else
/* on windows and linux simply use the file 'granny.csd' */
int result = csound->Compile("granny.csd");
#endif
    if(result)wxMessageBox(wxT("error compiling"));
    csThread = new CsoundPerformanceThread(csound);	
    csound->SetMessageCallback(csoundMessageCall);
    csound->SetChannel("freq", 1);
    csound->SetChannel("dens", 10);
    csound->SetChannel("ampoff", 1);
    csound->SetChannel("pitchoff", 1);
    csound->SetChannel("graindur", 0.5);   
}

void MyFrame::OnQuit(wxCommandEvent& event){
  	csound->Cleanup();
  	csThread->Stop();
  	csThread->Join();
 	delete csThread;
 	delete csound;	
 	csoundOutput->Show(false);	
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& event){
    wxMessageBox(wxT("Granular Synthesiser v.2"));
}

void MyFrame::StartStop(wxCommandEvent& event){
if(!strcmp(menu->GetLabel(400).mb_str(), "Start")){
	menu->SetLabel(400, wxT("Stop"));
	csThread->Play();
	timer->Start(1000);
	}
 else if(!strcmp(menu->GetLabel(400).mb_str(), "Stop")){
	menu->SetLabel(400, wxT("Start"));
	csThread->Pause();
	timer->Stop();
	} 
}

void MyFrame::ScrollEvent(wxScrollEvent& event)
{
wxObject* object = event.GetEventObject();
mySliderBox *sliderBox= (mySliderBox*)((wxWindow*)object)->GetParent();
sliderBox->textCtrl->ChangeValue(float2wxStr((sliderBox->slider->GetValue()/100.f)*(sliderBox->sliderRange)));
if(!strcmp("freq", sliderBox->slider->GetName().mb_str()))
	csound->SetChannel("freq", (float)(sliderBox->slider->GetValue()/100.f)*sliderBox->sliderRange);	
else if(!strcmp("dens", sliderBox->slider->GetName().mb_str()))
	csound->SetChannel("dens", (float)(sliderBox->slider->GetValue()/100.f)*sliderBox->sliderRange);
else if(!strcmp("ampoff", sliderBox->slider->GetName().mb_str()))
	csound->SetChannel("ampoff", (float)(sliderBox->slider->GetValue()/100.f)*sliderBox->sliderRange);
else if(!strcmp("pitchoff", sliderBox->slider->GetName().mb_str()))
	csound->SetChannel("pitchoff", (float)(sliderBox->slider->GetValue()/100.f)*sliderBox->sliderRange);
else if(!strcmp("gdur", sliderBox->slider->GetName().mb_str()))
	csound->SetChannel("graindur", (float)(sliderBox->slider->GetValue()/100.f)*sliderBox->sliderRange);
}

void csoundMessageCall(CSOUND* csound, int attr, const char* format, va_list valist)
{
   char* msg = (char*)malloc(16384);
   vsnprintf(msg, 16384, format, valist);//max mess length = 16384
   wxString test(msg, wxConvUTF8);
   str = test+str;
}
