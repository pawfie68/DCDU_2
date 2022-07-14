#include <wx/stattext.h>
#include "Communicate.h"

MainPanel::MainPanel(wxPanel* parent)
    : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X), 
        wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)*2/3), /*wxBORDER_SUNKEN*/ wxNO_BORDER )
{
    main_font = new wxFont(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");

    count_msg = 0;
    count_poe = 0;
    m_parent = parent;
    m_text_main = new wxStaticText(this, -1, wxT("MESSAGE FOR MID SCREEN\nWILL APPEAR HERE"), wxPoint(10, 20));
    m_text_main->SetFont(*main_font);
    m_text_main->SetForegroundColour(wxColor(255, 255, 255));

    msg_plus = new wxButton(this, ID_MSG_PLUS, wxT("MSG+"),
        wxPoint(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 - 100));
    msg_plus->SetBackgroundColour(wxColor(110,110,110));
    msg_plus->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_MSG_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainPanel::OnMsgPlus));

    msg_minus = new wxButton(this, ID_MSG_MINUS, wxT("MSG-"),
        wxPoint(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 - 50));
    msg_minus->SetBackgroundColour(wxColor(110,110,110));
    msg_minus->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_MSG_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainPanel::OnMsgMinus));

    m_POE_plus = new wxButton(this, ID_POE_PLUS, wxT("PGE +"),
        wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X)-100, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)*2/3-100));
    m_POE_plus->SetBackgroundColour(wxColor(110, 110, 110));
    m_POE_plus->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_POE_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainPanel::OnPOEPlus));

    m_POE_minus = new wxButton(this, ID_POE_MINUS, wxT("PGE -"),
        wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X)-100, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 - 50));
    m_POE_minus->SetBackgroundColour(wxColor(110, 110, 110));
    m_POE_minus->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_POE_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainPanel::OnPOEMinus));
}

void MainPanel::OnMsgPlus(wxCommandEvent& WXUNUSED(event))
{
    count_msg++;
    text_main = ("MESSAGE MSG+ RECEIVED");
    Communicate* comm = (Communicate*)m_parent->GetParent();
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_msg));
    comm->m_mp->m_text_main->SetLabel( text_main);
    comm->m_midp->m_text_mid->SetLabel(text_main);
}

void MainPanel::OnMsgMinus(wxCommandEvent& WXUNUSED(event))
{
    count_msg--;
    text_main = ("MESSAGE MSG- RECEIVED");
    Communicate* comm = (Communicate*)m_parent->GetParent();
    comm->m_mp->m_text_main->SetLabel(text_main);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_msg));
    comm->m_midp->m_text_mid->SetLabel(text_main);
}

void MainPanel::OnPOEPlus(wxCommandEvent& WXUNUSED(event))
{
    count_poe++;
    text_main = ("MESSAGE PGE+ RECEIVED");
    Communicate* comm = (Communicate*)m_parent->GetParent();
    comm->m_mp->m_text_main->SetLabel(text_main);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_poe));
    comm->m_midp->m_text_mid->SetLabel(text_main);
}

void MainPanel::OnPOEMinus(wxCommandEvent& WXUNUSED(event))
{
    count_poe--;
    text_main = ("MESSAGE PGE- RECEIVED");
    Communicate* comm = (Communicate*)m_parent->GetParent();
    comm->m_mp->m_text_main->SetLabel(text_main);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_poe));
    comm->m_midp->m_text_mid->SetLabel(text_main);
}

void LeftPanel::OnLeftUp(wxCommandEvent& WXUNUSED(event))
{
    count_l ++;
    text_l = "LEFT UP";
    //wxMessageBox(wxString("Left UP"));
    Communicate* comm = (Communicate*)m_parent->GetParent();
    comm->m_lp->m_text_l->SetLabel(text_l);
    comm->m_midp->m_text_mid->SetLabel(text_l);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_l));

}

void LeftPanel::OnLeftDown(wxCommandEvent& WXUNUSED(event))
{
    count_l --;
    text_l = "LEFT DOWN";
    Communicate* comm = (Communicate*)m_parent->GetParent();
    comm->m_lp->m_text_l->SetLabel(text_l);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_l));
    comm->m_midp->m_text_mid->SetLabel(text_l);
}

void RightPanel::OnRightUp(wxCommandEvent& WXUNUSED(event))
{
    count_r ++;
    text_r = "RIGHT UP";
    Communicate* comm = (Communicate*)m_parent->GetParent();
    comm->m_rp->m_text_r->SetLabel(text_r);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_r));
    comm->m_midp->m_text_mid->SetLabel(text_r);
}

void RightPanel::OnRightDown(wxCommandEvent& WXUNUSED(event))
{
    count_r --;
    text_r = "RIGHT DOWN";
    Communicate* comm = (Communicate*)m_parent->GetParent();
    comm->m_rp->m_text_r->SetLabel(text_r);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_r));
    comm->m_midp->m_text_mid->SetLabel(text_r);

}


RightPanel::RightPanel(wxPanel* parent) 
    : wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) *3/ 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)*2/3),
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X)/4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)/3), /*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
    right_font = new wxFont(38, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");

    count_r = 0;
    m_text = new wxStaticText(this, -1, wxT("0000000000000"), wxPoint((wxSystemSettings::GetMetric(wxSYS_SCREEN_X) * 4 / 5)/2, 10));
    m_text_r = new wxStaticText(this, -1, wxT("MESSAGE FOR RIGHT SCREEN\nWILL APPEAR HERE"), wxPoint(10, 30));
    m_text_r->SetFont(*right_font);

    m_text_r->SetForegroundColour(wxColor(52, 235, 161));
    m_text->SetForegroundColour(wxColor(255, 255, 255));

    m_right_up = new wxButton(this, ID_RU, wxT("R UP"),
        wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4 -100 
            , wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 150));
    m_right_up->SetBackgroundColour(wxColor(110, 110, 110));
    m_right_up->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_RU, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(RightPanel::OnRightUp));

    m_right_down = new wxButton(this, ID_RD, wxT("R DWN"),
        wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4 - 100, 
            wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 100));
    m_right_down->SetBackgroundColour(wxColor(110, 110, 110));
    m_right_down->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_RD, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(RightPanel::OnRightDown));
}

LeftPanel::LeftPanel(wxPanel* parent) 
    : wxPanel(parent, wxID_ANY, wxPoint(0, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3),
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3), 
        /*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
    left_font = new wxFont(38, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");
    count_l = 0;
    m_text_l = new wxStaticText(this, -1, wxT("MESSAGE FOR LEFT SCREEN\nWILL APPEAR HERE"), wxPoint(10, 20));
    m_text_l->SetFont(*left_font);
    m_text_l->SetForegroundColour(wxColor(52, 235, 161));;

    m_left_up = new wxButton(this, ID_LU, wxT("L UP"),
        wxPoint(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 150));
    Connect(ID_LU, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(LeftPanel::OnLeftUp));
    m_left_up->SetBackgroundColour(wxColor(110, 110, 110));
    m_left_up->SetForegroundColour(wxColor(*wxWHITE));
    m_left_down = new wxButton(this, ID_LD, wxT("L DWN"),
        wxPoint(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 100));
    Connect(ID_LD, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(LeftPanel::OnLeftDown));
    m_left_down->SetBackgroundColour(wxColor(110, 110, 110));
    m_left_down->SetForegroundColour(wxColor(*wxWHITE));
}

MidlePanel::MidlePanel(wxPanel* parent) 
    : wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) *2 / 3),
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) /2, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) *2/ 3), 
         /*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
    sizer_1 = new wxBoxSizer(wxVERTICAL);

    mid_font = new wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN, false, "Arial Rounded");
    text_mid = "BUTTON CLICKED: ";
    
    m_text_mid = new wxStaticText(this, -1, wxT("MESSAGE FOR MID SCREEN\nWILL APPEAR HERE"), wxPoint(10,10),
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2 -20, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 -20),
        wxEXPAND | wxALIGN_BOTTOM | wxBORDER_SUNKEN);

    //sizer_1->Add(m_text_mid,0, wxALL | wxALIGN_CENTER_HORIZONTAL, 200);
    m_text_mid->SetFont(*mid_font);
    
    m_text_mid->SetForegroundColour(wxColor(52, 235, 161));;
 
    count_m = 0;
}



PgCounterPanel::PgCounterPanel(wxPanel* parent)
    : wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X)*3 / 4 - 250, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) - 100),
        wxSize(200, 90),
        wxBORDER_SUNKEN  /*wxNO_BORDER*/)
{
    sizer_1 = new wxBoxSizer(wxVERTICAL);

    pg_font = new wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN, false, "Arial Rounded");
    

    m_text_pg = new wxStaticText(this, -1, wxT("PAGE COUNT\n\nWILL APPEAR HERE"), 
        wxPoint(0, 0),
        wxSize(190, 90),
        wxEXPAND | wxALIGN_CENTER | wxBORDER_SUNKEN);

    //sizer_1->Add(m_text_mid,0, wxALL | wxALIGN_CENTER_HORIZONTAL, 200);
    m_text_pg->SetFont(*pg_font);

    m_text_pg->SetForegroundColour(wxColor(120, 120, 120));;

    count_pg = 0;
}