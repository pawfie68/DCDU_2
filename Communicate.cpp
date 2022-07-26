#include "Communicate.h"
#include <wx/statline.h>

//this frame will cover whole screen of the LCD
Communicate::Communicate(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition,
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X), wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)))
{
    //defile the parent and setup background color of whole app, setup full screen with no borders
    m_parent = new wxPanel(this, wxID_ANY);
    m_parent->SetBackgroundColour(wxColor(*wxBLACK));
    m_parent->SetWindowStyle(wxFULLSCREEN_ALL && wxBORDER_NONE);
    
    //display static lines separating text areas
    wxStaticLine* horizontal = new wxStaticLine(m_parent, wxID_ANY, wxPoint(30,  wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 - 5), 
       wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) - 60, 10));
    wxStaticLine* vertical_l = new wxStaticLine(m_parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4 - 5,
        wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 + 30), wxSize(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 60));
    wxStaticLine* vertical_2 = new wxStaticLine(m_parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) * 3/4 -5, 
        wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 + 30), wxSize(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 60));

    //main sizer of the app
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

    //initializaton of all panels
    m_mp = new MainPanel(m_parent);
    m_rp = new RightPanel(m_parent);
    m_lp = new LeftPanel(m_parent);
    m_midp = new MidlePanel(m_parent);
    m_pgp = new PgCounterPanel(m_parent);

    //set defoult font color for all panels to white
    m_rp->SetForegroundColour(wxColor(255, 255, 255));
    m_mp->SetForegroundColour(wxColor(255, 255, 255));
    m_lp->SetForegroundColour(wxColor(255, 255, 255));
    m_midp->SetForegroundColour(wxColor(255, 255, 255));
    m_pgp->SetForegroundColour(wxColor(255, 255, 255));
    
    //add panels to the main sizer
    hbox->Add(m_lp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(m_rp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(m_mp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(m_midp, 1, wxEXPAND | wxALL, 5);
    hbox->Add(m_pgp, 1, wxEXPAND | wxALL, 5);
    //m_parent->SetSizer(hbox);
     
    int count_msg;
    int count_poe;

}



