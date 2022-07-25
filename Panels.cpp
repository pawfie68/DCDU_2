#include <wx/stattext.h>
#include "Communicate.h"


//MainPanel scaling with the screen resolution

MainPanel::MainPanel(wxPanel* parent)
    : wxPanel(parent, -1, wxPoint(-1, -1), wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
        wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3), /*wxBORDER_SUNKEN*/ wxNO_BORDER)
{
    //font setting
    main_font = new wxFont(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");
    //counters for handling number of clicks -> for future use
    count_msg = 0;
    count_poe = 0;
    //setting the parent for enabling communication between panels
    m_parent = parent;
    //display the standby message
    m_text_main = new wxStaticText(this, -1, wxT("MESSAGE FOR MID SCREEN\nWILL APPEAR HERE"), wxPoint(10, 20));
    m_text_main->SetFont(*main_font);
    m_text_main->SetForegroundColour(wxColor(255, 255, 255));

    //creating button for MSG+ and connect it to the proper event
    msg_plus = new wxButton(this, ID_MSG_PLUS, wxT("MSG+"),
        wxPoint(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 - 100));
    msg_plus->SetBackgroundColour(wxColor(110, 110, 110));
    msg_plus->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_MSG_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainPanel::OnMsgPlus));
    //creating button for MSG- and connect it to the proper event
    msg_minus = new wxButton(this, ID_MSG_MINUS, wxT("MSG-"),
        wxPoint(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 - 50));
    msg_minus->SetBackgroundColour(wxColor(110, 110, 110));
    msg_minus->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_MSG_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainPanel::OnMsgMinus));
    //creating button for POE+ and connect it to the proper event
    m_POE_plus = new wxButton(this, ID_POE_PLUS, wxT("PGE +"),
        wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) - 100, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 - 100));
    m_POE_plus->SetBackgroundColour(wxColor(110, 110, 110));
    m_POE_plus->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_POE_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainPanel::OnPOEPlus));
    //creating button for POE+ and connect it to the proper event
    m_POE_minus = new wxButton(this, ID_POE_MINUS, wxT("PGE -"),
        wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) - 100, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3 - 50));
    m_POE_minus->SetBackgroundColour(wxColor(110, 110, 110));
    m_POE_minus->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_POE_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(MainPanel::OnPOEMinus));

    //////////////////////////////////////////////////////////////////////////
    // TCP/IP part////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    
  
   //  Create the socket
    m_sock = new wxSocketClient();


     //Setup the event handler and subscribe to most events
        m_sock->SetEventHandler(*this, SOCKET_ID);
        Connect(SOCKET_ID, wxEVT_SOCKET,
            wxSocketEventHandler(MainPanel::OnSocketEvent));

    //setup some flags to maintain socket events and enable notifications
        m_sock->SetNotify(wxSOCKET_CONNECTION_FLAG |
            wxSOCKET_INPUT_FLAG |
            wxSOCKET_LOST_FLAG);
        m_sock->Notify(true);
   
    //connect to IPV4 type adress 
        OpenConnection(wxSockAddress::IPV4);

}

MainPanel::~MainPanel()
{
    // No delayed deletion here, as the frame is dying anyway
    delete m_sock;
}

   void MainPanel::OnSocketEvent(wxSocketEvent& event)
    {
       //wxBuffer to collect data from socket

        wxCharBuffer buf2 = "";

        //here we will handle the socket events
        switch (event.GetSocketEvent())
        {
            //when data arrived
        case wxSOCKET_INPUT:
            //reading 5 characters (1 should be enough, but to be safe, larger buffor was applied)
            //and save them to the buffer buf2
            m_sock->Read(buf2.data(), 5);
        
            //write back data from buf2, 
            m_sock->Write(buf2.data(), wxStrlen(buf2.data()));
            //and display them on the scren, clean screen first
            m_text_main->SetLabel("");
            m_text_main->SetLabel(buf2.data());
            break;

            //when we lost connection with server
        case wxSOCKET_LOST:
           
          m_text_main->SetLabel("Socket connection was unexpectedly lost.");
            break;
            //when we establish connection with server
        case wxSOCKET_CONNECTION:
            m_text_main->SetForegroundColour(wxColor(0, 255, 0));
          m_text_main->SetLabel("... socket is now connected.");
            break;

            //Other events will trigger this action
        default:
          m_text_main->SetLabel("Unknown socket event!!!");
           break;
        }
    }

    void MainPanel::OnOpenConnection(wxCommandEvent& WXUNUSED(event))
    {
        OpenConnection(wxSockAddress::IPV4);
    }


    void MainPanel::OpenConnection(wxSockAddress::Family family)
    {
       wxUnusedVar(family); // unused in !wxUSE_IPV6 case
        wxIPaddress* addr;
        wxIPV4address addr4;

            addr = &addr4;

    // Ask user for server address
        wxString hostname = wxGetTextFromUser(
            _("Enter the address of the wxSocket demo server:"),
            _("Connect ..."),
            _("localhost"));
        if (hostname.empty())
            return;
    //hostname = "www.google.pl";
        addr->Hostname(hostname);
        addr->Service(5060);
        m_sock->Connect(*addr, false);
    // we connect asynchronously and will get a wxSOCKET_CONNECTION event when
    // the connection is really established
    
    // if you want to make sure that connection is established right here you
    // could call WaitOnConnect(timeout) instead
        wxMessageBox("Trying to connect to %s:%d"/* ,hostname, addr->Service()*/);
    
        if (m_sock->IsConnected())
        {
            m_text_main->SetLabelText(wxT("connected"));
        }
        else
        {
            m_text_main->SetLabelText(wxT("connection error"));
        }
        m_busy = false;


        m_sock->Connect(*addr, false);
    }


//Events on buttons in main panel//

void MainPanel::OnMsgPlus(wxCommandEvent& WXUNUSED(event))
{
    
    count_msg++;
   
    Communicate* comm = (Communicate*)m_parent->GetParent();
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_msg));
    //send text to different text boxes
    comm->m_mp->m_text_main->SetLabel( text_main);
    comm->m_midp->m_text_mid->SetLabel(text_main);
}



void MainPanel::OnMsgMinus(wxCommandEvent& WXUNUSED(event))
{
    count_msg--;
    text_main = ("MESSAGE MSG- RECEIVED");
    Communicate* comm = (Communicate*)m_parent->GetParent();
    //send text to different text boxes
    comm->m_mp->m_text_main->SetLabel(text_main);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_msg));
    comm->m_midp->m_text_mid->SetLabel(text_main);
}

void MainPanel::OnPOEPlus(wxCommandEvent& WXUNUSED(event))
{
    count_poe++;
    text_main = ("MESSAGE PGE+ RECEIVED");
    Communicate* comm = (Communicate*)m_parent->GetParent();
    //send text to different text boxes
    comm->m_mp->m_text_main->SetLabel(text_main);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_poe));
    comm->m_midp->m_text_mid->SetLabel(text_main);
}

void MainPanel::OnPOEMinus(wxCommandEvent& WXUNUSED(event))
{
    count_poe--;
    text_main = ("MESSAGE PGE- RECEIVED");
    Communicate* comm = (Communicate*)m_parent->GetParent();
    //send text to different text boxes
    comm->m_mp->m_text_main->SetLabel(text_main);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_poe));
    comm->m_midp->m_text_mid->SetLabel(text_main);
}



//RightPanel setup

RightPanel::RightPanel(wxPanel* parent) 
    : wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) *3/ 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)*2/3),
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X)/4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)/3), /*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
    //font setup
    right_font = new wxFont(38, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");

    count_r = 0;
    //text setup
    m_text = new wxStaticText(this, -1, wxT("0000000000000"), wxPoint((wxSystemSettings::GetMetric(wxSYS_SCREEN_X) * 4 / 5)/2, 10));
    m_text_r = new wxStaticText(this, -1, wxT("MESSAGE FOR RIGHT SCREEN\nWILL APPEAR HERE"), wxPoint(10, 30));
    m_text_r->SetFont(*right_font);

    m_text_r->SetForegroundColour(wxColor(52, 235, 161));
    m_text->SetForegroundColour(wxColor(255, 255, 255));

    //creating button for RightUP and connect it to the proper event
    m_right_up = new wxButton(this, ID_RU, wxT("R UP"),
        wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4 -100 
            , wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 150));
    m_right_up->SetBackgroundColour(wxColor(110, 110, 110));
    m_right_up->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_RU, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(RightPanel::OnRightUp));

    //creating button for RoghtDWN and connect it to the proper event
    m_right_down = new wxButton(this, ID_RD, wxT("R DWN"),
        wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4 - 100, 
            wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 100));
    m_right_down->SetBackgroundColour(wxColor(110, 110, 110));
    m_right_down->SetForegroundColour(wxColor(*wxWHITE));
    Connect(ID_RD, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(RightPanel::OnRightDown));

    m_IP_value = 0;
}

//Events on buttons in right panel//

void RightPanel::OnRightUp(wxCommandEvent& WXUNUSED(event))
{
    if (m_IP_value > 9)
    {
        m_IP_value = 0;
    }
    else
    {
        m_IP_value++;
    }

    count_r++;
    text_r = "RIGHT UP";
    Communicate* comm = (Communicate*)m_parent->GetParent();
    //send text to different text boxes
    comm->m_rp->m_text_r->SetLabel(text_r);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_r));
    comm->m_midp->m_text_mid->SetLabel(text_r);
}

void RightPanel::OnRightDown(wxCommandEvent& WXUNUSED(event))
{
    if (m_IP_value > 9)
    {
        m_IP_value = 0;
    }
    else
    {
        m_IP_value++;
    }
    count_r--;
    text_r = "RIGHT DOWN";
    Communicate* comm = (Communicate*)m_parent->GetParent();
    //send text to different text boxes
    comm->m_rp->m_text_r->SetLabel(text_r);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_r));
    comm->m_midp->m_text_mid->SetLabel(text_r);

    /// proceed with menue on right button + F1 later
    /// /
    /// if (wxGetKeyState(WXK_F1)) wxMessageBox("key 1");
    /// 
    
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

    //creating button for LeftUP and connect it to the proper event
    m_left_up = new wxButton(this, ID_LU, wxT("L UP"),
        wxPoint(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 150));
    Connect(ID_LU, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(LeftPanel::OnLeftUp));
    m_left_up->SetBackgroundColour(wxColor(110, 110, 110));
    m_left_up->SetForegroundColour(wxColor(*wxWHITE));
    //creating button for LeftUP and connect it to the proper event
    m_left_down = new wxButton(this, ID_LD, wxT("L DWN"),
        wxPoint(10, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 100));
    Connect(ID_LD, wxEVT_COMMAND_BUTTON_CLICKED,
        wxCommandEventHandler(LeftPanel::OnLeftDown));
    m_left_down->SetBackgroundColour(wxColor(110, 110, 110));
    m_left_down->SetForegroundColour(wxColor(*wxWHITE));

    m_IP_index = 0;
}

//Events on buttons in left panel//

void LeftPanel::OnLeftUp(wxCommandEvent& WXUNUSED(event))
{
    if (m_IP_index > 10)
    {
        m_IP_index = 0;
    }
    else
    {
        m_IP_index++;
    }
    
    count_l++;
    text_l = "LEFT UP";
    Communicate* comm = (Communicate*)m_parent->GetParent();
    //send text to different text boxes
    comm->m_lp->m_text_l->SetLabel(text_l);
    comm->m_midp->m_text_mid->SetLabel(text_l);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_l));

}

void LeftPanel::OnLeftDown(wxCommandEvent& WXUNUSED(event))
{
    if (m_IP_index < 0)
    {
        m_IP_index = 0;
    }
    else
    {
        m_IP_index--;
    }

    count_l--;
    text_l = "LEFT DOWN";
    Communicate* comm = (Communicate*)m_parent->GetParent();
    //send text to different text boxes
    comm->m_lp->m_text_l->SetLabel(text_l);
    comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_l));
    comm->m_midp->m_text_mid->SetLabel(text_l);
}


MidlePanel::MidlePanel(wxPanel* parent) 
    : wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) *2 / 3),
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) /2, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) *2/ 3), 
         /*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
    //font setup
    mid_font = new wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN, false, "Arial Rounded");
    text_mid = "BUTTON CLICKED: ";
    
    //textbox for midle screen
    m_text_mid = new wxStaticText(this, -1, wxT("MESSAGE FOR MID SCREEN\nWILL APPEAR HERE"), wxPoint(10,10),
        wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2 -20, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 -20),
        wxEXPAND | wxALIGN_CENTER );

    m_text_mid->SetFont(*mid_font);
    
    m_text_mid->SetForegroundColour(wxColor(52, 235, 161));;
 
    count_m = 0;
}


//additional screen -> will display only page counter in the future
//adjustment of size will be needed on final device
PgCounterPanel::PgCounterPanel(wxPanel* parent)
    : wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X)*3 / 4 - 250, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) - 100),
        wxSize(200, 90),
        wxBORDER_SUNKEN  /*wxNO_BORDER*/)
{
   //font setup
    pg_font = new wxFont(15, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN, false, "Arial Rounded");
    
    //small textbox for PG counter screen
    //adjustment of size will be needed on final device
    m_text_pg = new wxStaticText(this, -1, wxT("PAGE COUNT\n\nWILL APPEAR HERE"), 
        wxPoint(0, 0),
        wxSize(190, 90),
        wxEXPAND | wxALIGN_CENTER | wxBORDER_SUNKEN);

    m_text_pg->SetFont(*pg_font);

    m_text_pg->SetForegroundColour(wxColor(120, 120, 120));;

    count_pg = 0;
}