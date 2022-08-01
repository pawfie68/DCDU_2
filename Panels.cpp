#include <wx/stattext.h>
#include "Communicate.h"


#include "wx/config.h"

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif

#include "Button_handlers.h"
#include "Net.h"

//MainPanel scaling with the screen resolution

MainPanel::MainPanel(wxPanel* parent)
	: wxPanel(parent, -1, wxPoint(-1, -1), wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
		wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3), /*wxBORDER_SUNKEN*/ wxNO_BORDER )
{
	
	//font setting
	main_font = new wxFont(50, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial"/*"New Font"*/);
	//counters for handling number of clicks -> for future use
	//main_font->AddPrivateFont("H:\\Repos\\DCDU-master\\DCDU_REGULAR.ttf");
	//setting the parent for enabling communication between panels
	m_parent = parent;
	//display the standby message
	m_text_main = new wxStaticText(this, -1, wxT(""), wxPoint(10, 20));
	
	m_text_main->SetFont(*main_font);
	m_text_main->SetForegroundColour(wxColor(0, 255, 255));
	m_text_main->Wrap(wxSystemSettings::GetMetric(wxSYS_SCREEN_X));
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


//creating button for POE+ and connect it to the proper event
m_print = new wxButton(this, ID_PRINT, wxT("PRINT"),
	wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) - 100, 50));
m_print->SetBackgroundColour(wxColor(110, 110, 110));
m_print->SetForegroundColour(wxColor(*wxWHITE));
Connect(ID_PRINT, wxEVT_COMMAND_BUTTON_CLICKED,
	wxCommandEventHandler(MainPanel::OnPrintEvent));

Connect(ID_COMBINED, wxEVT_COMMAND_BUTTON_CLICKED,
	wxCommandEventHandler(MainPanel::OnPrintEvent));


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


Communicate* comm = (Communicate*)m_parent->GetParent();

if (comm->m_ip->Config_flag == false)
{
	
	if (OpenConnection(wxSockAddress::IPV4))
	{
		
	}
	//connect to IPV4 type adress 
	//OpenConnection(wxSockAddress::IPV4, addr);
	// 
	//comm->m_ip->Hide();
	
}
else
{
	// we want to set focus on all elements of IP panel one by one, to display them on the front of the main app window
	comm->m_ip->SetFocus();
	comm->m_ip->m_ip_2->SetFocus();
	comm->m_ip->m_ip_3->SetFocus(); comm->m_ip->m_ip_3->SetFocus();
	comm->m_ip->m_ip_5->SetFocus(); comm->m_ip->m_ip_6->SetFocus();
	comm->m_ip->m_ip_7->SetFocus(); comm->m_ip->m_ip_8->SetFocus();
	comm->m_ip->m_ip_9->SetFocus(); comm->m_ip->m_ip_10->SetFocus();
	comm->m_ip->m_ip_11->SetFocus(); comm->m_ip->m_ip_12->SetFocus();
	comm->m_ip->m_ip_13->SetFocus(); comm->m_ip->m_ip_14->SetFocus();
	comm->m_ip->m_ip_15->SetFocus(); comm->m_ip->m_ip_16->SetFocus();
	comm->m_ip->m_ip_17->SetFocus(); comm->m_ip->m_ip_1->SetFocus();
}

}


MainPanel::~MainPanel()
{
	delete m_sock;
}

///////////////////////////////////////////////// IpPanel //////////////////////////////////////////////////
/// ____________________________________________________________________________________________________ ///
/// This panel will show up when no config file is present in the system, or in case when data stored in ///
/// the config file are corrupted, or when we wish it to display------------------------------------ ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////

IpPanel::IpPanel(wxPanel* parent)
	: wxPanel(parent, -1, wxPoint((wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2 - wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4), -1),
		wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2,
			wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 5), /*wxBORDER_SUNKEN*/  wxSTAY_ON_TOP)
{
	//communicate class is the parent class of all panels do we can use it to communicate between all elements
	//of the UI
	Communicate* comm = (Communicate*)m_parent->GetParent();

	//some auxiliary values to deal with the values readed from config file (UNIX) registry (Windows)
	
	wxConfigBase* pConfig = wxConfigBase::Get();

	wxIPaddress* addr;
	wxIPV4address addr4;
	addr = &addr4;


	ip_from_cfg = pConfig->Read("/Controls/m_IP");
	port_from_cfg = pConfig->Read("/Controls/m_PORT");
	hostname = ip_from_cfg;
	port = port_from_cfg;

	if (ip_from_cfg.IsEmpty() ||
		port_from_cfg.IsEmpty())
	{
		Config_flag = true;
	}

	//If there are already Ip and HOST values in the config file
	else
	{
		addr->Hostname(port_from_cfg);
		addr->Service(port_from_cfg);

		Config_flag = false;
	
	}

	/// ========================================================================================================================================
	/// When config is needed Config_flag == true and we run the proper functions
	/// else, do nothing
	NetConfig();


		//===============================================================================================================================
		// and now is a time to deal with visual aspects of IP panel
		// at the beggining we want to know where to place the IP screen -> in we will start at the 1/4 of screen resolution
		//===============================================================================================================================
		int start_position = wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4;
		// and define some description of UI to the user
		wxString description = "<- [MSG-] / [MSG+] -> \n"  "+value [POE+] \n -value [POE-] \n"
			"sign empty under the 0\n"
			"[PRINT] - save IP/HOST";

		//Here we can set up the font properties for the all IP panel
		ip_font = new wxFont(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");

		//===============================================================================================================================
		//and one after another we can display the IP/HOST values for every single window of IP Panel
		//we can also change color of font and background of small IP textboxes
		m_ip_1 = new wxTextCtrl(this, -1, ip_to_display[0], wxPoint(start_position - 330, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_1->SetFont(*ip_font);
		m_ip_1->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_1->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_2 = new wxTextCtrl(this, -1, ip_to_display[1], wxPoint(start_position - 290, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_2->SetFont(*ip_font);
		m_ip_2->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_2->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_3 = new wxTextCtrl(this, -1, ip_to_display[2], wxPoint(start_position - 250, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_3->SetFont(*ip_font);
		m_ip_3->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_3->SetForegroundColour(wxColor(0, 0, 0));

		dot1 = new wxStaticText(this, -1, wxT("."), wxPoint(start_position - 215, 30), wxSize(5, 50), wxNO_BORDER);
		dot1->SetFont(*ip_font);
		dot1->SetBackgroundColour(wxColor(0, 0, 0));
		dot1->SetForegroundColour(wxColor(255, 255, 255));

		m_ip_4 = new wxTextCtrl(this, -1, ip_to_display[4], wxPoint(start_position - 205, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_4->SetFont(*ip_font);
		m_ip_4->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_4->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_5 = new wxTextCtrl(this, -1, ip_to_display[5], wxPoint(start_position - 165, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_5->SetFont(*ip_font);
		m_ip_5->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_5->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_6 = new wxTextCtrl(this, -1, ip_to_display[6], wxPoint(start_position - 125, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_6->SetFont(*ip_font);
		m_ip_6->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_6->SetForegroundColour(wxColor(0, 0, 0));

		dot2 = new wxStaticText(this, -1, wxT("."), wxPoint(start_position - 90, 30), wxSize(5, 50), wxNO_BORDER);
		dot2->SetFont(*ip_font);
		dot2->SetBackgroundColour(wxColor(0, 0, 0));
		dot2->SetForegroundColour(wxColor(255, 255, 255));

		m_ip_7 = new wxTextCtrl(this, -1, ip_to_display[8], wxPoint(start_position - 80, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_7->SetFont(*ip_font);
		m_ip_7->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_7->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_8 = new wxTextCtrl(this, -1, ip_to_display[9], wxPoint(start_position - 40, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_8->SetFont(*ip_font);
		m_ip_8->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_8->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_9 = new wxTextCtrl(this, -1, ip_to_display[10], wxPoint(start_position, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_9->SetFont(*ip_font);
		m_ip_9->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_9->SetForegroundColour(wxColor(0, 0, 0));

		dot2 = new wxStaticText(this, -1, wxT("."), wxPoint(start_position + 35, 30), wxSize(5, 50), wxNO_BORDER);
		dot2->SetFont(*ip_font);
		dot2->SetBackgroundColour(wxColor(0, 0, 0));
		dot2->SetForegroundColour(wxColor(255, 255, 255));

		m_ip_10 = new wxTextCtrl(this, -1, ip_to_display[12], wxPoint(start_position + 45, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_10->SetFont(*ip_font);
		m_ip_10->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_10->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_11 = new wxTextCtrl(this, -1, ip_to_display[13], wxPoint(start_position + 85, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_11->SetFont(*ip_font);
		m_ip_11->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_11->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_12 = new wxTextCtrl(this, -1, ip_to_display[14], wxPoint(start_position + 125, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_12->SetFont(*ip_font);
		m_ip_12->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_12->SetForegroundColour(wxColor(0, 0, 0));

		semicolon = new wxStaticText(this, -1, wxT(":"), wxPoint(start_position + 160, 20), wxSize(5, 50), wxNO_BORDER);
		semicolon->SetFont(*ip_font);
		semicolon->SetBackgroundColour(wxColor(0, 0, 0));
		semicolon->SetForegroundColour(wxColor(255, 255, 255));

		m_ip_13 = new wxTextCtrl(this, -1, port_to_dispaly[0], wxPoint(start_position + 175, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_13->SetFont(*ip_font);
		m_ip_13->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_13->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_14 = new wxTextCtrl(this, -1, port_to_dispaly[1], wxPoint(start_position + 210, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_14->SetFont(*ip_font);
		m_ip_14->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_14->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_15 = new wxTextCtrl(this, -1, port_to_dispaly[2], wxPoint(start_position + 245, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_15->SetFont(*ip_font);
		m_ip_15->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_15->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_16 = new wxTextCtrl(this, -1, port_to_dispaly[3], wxPoint(start_position + 280, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_16->SetFont(*ip_font);
		m_ip_16->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_16->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_17 = new wxTextCtrl(this, -1, port_to_dispaly[4], wxPoint(start_position + 315, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_17->SetFont(*ip_font);
		m_ip_17->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_17->SetForegroundColour(wxColor(0, 0, 0));

		ip_setup_desc = new wxStaticText(this, -1, wxT(""), wxPoint(start_position - 500, 100), wxSize(400, 100), wxNO_BORDER);
		ip_setup_desc->SetFont(*ip_font);
		ip_setup_desc->SetBackgroundColour(wxColor(0, 0, 0));
		ip_setup_desc->SetForegroundColour(wxColor(0, 200, 100));
		//===============================================================================================================================
		//and display the buttons description 
		ip_setup_desc->SetLabel(description);	
}

IpPanel::~IpPanel()
{
	//when IpPanel is destroyed we want to save our IP/HOST data to proper file 
	/* communicate class to handle data transfer between the panels */
	Communicate* comm = (Communicate*)m_parent->GetParent();

	wxConfigBase* pConfig = wxConfigBase::Get();
	if (pConfig == NULL)
		return;

	// save the control's values to the config
	pConfig->Write("/Controls/m_IP", hostname);
	pConfig->Write("/Controls/m_PORT", port);
	pConfig->Write("/TestValue", "A test va88");
}

//======================================================================================================================================
// In this part we will deal with the constructors of the auxilary panels and events connected to them
//======================================================================================================================================
// 
//RightPanel setup
//As a condition we provide the size of the panel in relation to the screen resolution, and relative point at which we will start to draw

RightPanel::RightPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) * 3 / 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3),
		wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3), /*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
	//font setup
	right_font = new wxFont(38, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	count_r = 0; // this counter will be removed soon
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//text setup
	m_text = new wxStaticText(this, -1, wxT("0000000000000"), wxPoint((wxSystemSettings::GetMetric(wxSYS_SCREEN_X) * 4 / 5) / 2, 10));
	m_text_r = new wxStaticText(this, -1, wxT("MESSAGE FOR RIGHT SCREEN\nWILL APPEAR HERE"), wxPoint(10, 30));
	m_text_r->SetFont(*right_font);

	m_text_r->SetForegroundColour(wxColor(52, 235, 161));
	m_text->SetForegroundColour(wxColor(255, 255, 255));

	//creating button for RightUP and connect it to the proper event
	m_right_up = new wxButton(this, ID_RU, wxT("R UP"),
		wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4 - 100
			, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 150));
	m_right_up->SetBackgroundColour(wxColor(110, 110, 110));
	m_right_up->SetForegroundColour(wxColor(*wxWHITE));
	Connect(ID_RU, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(RightPanel::OnRightUp));

	//creating button for RightDWN and connect it to the proper event
	m_right_down = new wxButton(this, ID_RD, wxT("R DWN"),
		wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4 - 100,
			wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 100));
	m_right_down->SetBackgroundColour(wxColor(110, 110, 110));
	m_right_down->SetForegroundColour(wxColor(*wxWHITE));
	Connect(ID_RD, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(RightPanel::OnRightDown));
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	m_IP_value = 0; //the name of this counters will be changed
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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


//middle panel will be modified only by other events, doesnt have any event by itself

MidlePanel::MidlePanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3),
		wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3),
		/*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
	//font setup
	mid_font = new wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN, false, "Arial Rounded");
	text_mid = "BUTTON CLICKED: ";

	//textbox for midle screen
	m_text_mid = new wxStaticText(this, -1, wxT("MESSAGE FOR MID SCREEN\nWILL APPEAR HERE"), wxPoint(10, 10),
		wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2 - 20, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3 - 20),
		wxEXPAND | wxALIGN_CENTER);

	m_text_mid->SetFont(*mid_font);

	m_text_mid->SetForegroundColour(wxColor(52, 235, 161));;

	count_m = 0;
}


//additional screen -> will display only page counter in the future
//adjustment of size will be needed on final device

PgCounterPanel::PgCounterPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) * 3 / 4 - 250, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) - 100),
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

