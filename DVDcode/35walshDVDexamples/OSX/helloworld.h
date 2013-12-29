/*********************** helloworld.h **************************/
#ifndef MYFRAME_H
#define MYFRAME_H
#include "wx/wx.h" 
 

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

    DECLARE_EVENT_TABLE()

enum
{
    ID_Quit = 100,
    ID_About = 200,
};

};
#endif
