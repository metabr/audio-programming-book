/******************** helloworld.cpp **************************/
#include "helloworld.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
   MyFrame *frame = new MyFrame(wxT("Hello Audio World"), wxPoint(50,50), wxSize(450,340));
   frame->Show(TRUE);
   return TRUE;
} 

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
END_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append( ID_About, wxT("&About..."));
    menuFile->Append( ID_Quit, wxT("E&xit"));
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, wxT("&File"));
    wxFrame::SetMenuBar( menuBar );
}

void MyFrame::OnQuit(wxCommandEvent& event)
{
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(wxT("Hello Audio World2!"));
}
