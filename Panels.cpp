#include <wx/stattext.h>
#include "Communicate.h"


#include "wx/config.h"

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif


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
	delete m_sock;
}

///////////////////////////////////////////////// IpPanel //////////////////////////////////////////////////
/// ____________________________________________________________________________________________________ ///
/// This panel will show up when no config file is present in the system, or in case when data stored in ///
/// the config file are corrupted ---------------------------------------------------------------------- ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////

IpPanel::IpPanel(wxPanel* parent)
	: wxPanel(parent, -1, wxPoint(-1, -1), wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X),
		wxSystemSettings::GetMetric(wxSYS_SCREEN_Y)/5), /*wxBORDER_SUNKEN*/  wxSTAY_ON_TOP)
{
	m_ip_1 = new wxTextCtrl(this, -1, wxT(""), wxPoint(10, 20), wxSize(50, 50));
	m_ip_1->SetBackgroundColour(wxColor(0, 50, 0));
	m_ip_1->SetForegroundColour(wxColor(255, 255, 255));
	
	m_ip_2 = new wxTextCtrl(this, -1, wxT(""), wxPoint(70, 20), wxSize(50, 50));
	m_ip_2->SetBackgroundColour(wxColor(0, 50, 0));
	m_ip_2->SetForegroundColour(wxColor(255, 255, 255));
	
	m_ip_3 = new wxTextCtrl(this, -1, wxT(""), wxPoint(130, 20), wxSize(50, 50));
	m_ip_3->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_3->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_4 = new wxTextCtrl(this, -1, wxT(""), wxPoint(190, 20), wxSize(50, 50));
	m_ip_4->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_4->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_5 = new wxTextCtrl(this, -1, wxT(""), wxPoint(250, 20), wxSize(50, 50));
	m_ip_5->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_5->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_6 = new wxTextCtrl(this, -1, wxT(""), wxPoint(310, 20), wxSize(50, 50));
	m_ip_6->SetBackgroundColour(wxColor(0, 50, 0));
	m_ip_6->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_7 = new wxTextCtrl(this, -1, wxT(""), wxPoint(370, 20), wxSize(50, 50));
	m_ip_7->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_7->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_8 = new wxTextCtrl(this, -1, wxT(""), wxPoint(430, 20), wxSize(50, 50));
	m_ip_8->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_8->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_9 = new wxTextCtrl(this, -1, wxT(""), wxPoint(490, 20), wxSize(50, 50));
	m_ip_9->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_9->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_10 = new wxTextCtrl(this, -1, wxT(""), wxPoint(550, 20), wxSize(50, 50));
	m_ip_10->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_10->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_11 = new wxTextCtrl(this, -1, wxT(""), wxPoint(610, 20), wxSize(50, 50));
	m_ip_11->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_11->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_12 = new wxTextCtrl(this, -1, wxT(""), wxPoint(670, 20), wxSize(50, 50));
	m_ip_12->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_12->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_13 = new wxTextCtrl(this, -1, wxT(""), wxPoint(10, 70), wxSize(50, 50));
	m_ip_13->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_13->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_14 = new wxTextCtrl(this, -1, wxT(""), wxPoint(70, 70), wxSize(50, 50));
	m_ip_14->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_14->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_15 = new wxTextCtrl(this, -1, wxT(""), wxPoint(130, 70), wxSize(50, 50));
	m_ip_15->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_15->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_16 = new wxTextCtrl(this, -1, wxT(""), wxPoint(190, 70), wxSize(50, 50));
	m_ip_16->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_16->SetForegroundColour(wxColor(255, 255, 255));

	m_ip_17 = new wxTextCtrl(this, -1, wxT(""), wxPoint(250, 70), wxSize(50, 50));
	m_ip_17->SetBackgroundColour(wxColor(0, 0, 0));
	m_ip_17->SetForegroundColour(wxColor(255, 255, 255));

	wxString ip_from_user = m_ip_1->GetValue() + m_ip_2->GetValue() + m_ip_3->GetValue() + "." 
		+ m_ip_4->GetValue() + m_ip_5->GetValue() + m_ip_6->GetValue() + "."
		+ m_ip_7->GetValue() + m_ip_8->GetValue() + m_ip_9->GetValue() + "."
		+ m_ip_10->GetValue() + m_ip_11->GetValue() + m_ip_12->GetValue();
	wxString port_from_user = m_ip_13->GetValue() + m_ip_14->GetValue() + m_ip_15->GetValue()
		+ m_ip_16->GetValue() + m_ip_17->GetValue();

	Communicate* comm = (Communicate*)m_parent->GetParent();
	comm->hbox->Add(m_ip_17);

	//m_text_ip = new wxStaticText(this, -1, wxT("MESSAGE FOR MID SCREEN\nWILL APPEAR HERE"), wxPoint(10, 20));
	//m_text_ip->SetFont(*main_font);
	//m_text_ip->SetForegroundColour(wxColor(255, 255, 255));
	//m_text_ip->Show();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//config
	// restore the control's values from the config

	// NB: in this program, the config object is already created at this moment
	// because we had called Get() from MyApp::OnInit(). However, if you later
	// change the code and don't create it before this line, it won't break
	// anything - unlike if you manually create wxConfig object with Create()
	// or in any other way (then you must be sure to create it before using it!).
	wxConfigBase* pConfig = wxConfigBase::Get();
	wxIPaddress* addr;
	wxIPV4address addr4;
	addr = &addr4;
	//comm->m_mp->addr = &addr4;
	//IP and PORT stored in registy->windows (/HKEY_CURRENT_USER/Software/YourVendor/YourApp)
	// propably /user/lib/local/Skalarki on unix (or in the app root location)
	// 

	/*Set up the config directory*/
	pConfig->SetPath("/Controls");

	/*we need to check if there are already values stored in this location*/
	/*If they are empty, reed them from the user*/

	if (pConfig->Read("m_IP").IsEmpty() ||
		pConfig->Read("m_PORT").IsEmpty())
	{
		Config_flag = true;
		//// Ask user for server address
		////m_text_ip->SetLabel("test");
		//hostname = wxGetTextFromUser(
		//	_("Enter the address of the server:"),
		//	_("Connect ..."),
		//	_("192.168.0.31"));
		//if (hostname.empty())
		//	return;

		//// Ask user for server port
		//port = wxGetTextFromUser(
		//	_("Enter the Port of the server:"),
		//	_("Connect ..."),
		//	_("5060"));
		//if (port.empty())
		//	return;
		///// <need some function for validation of ip adress and port values>
		///// //////////////////////////////////////////////////////////////// ///
		///// 
		//pConfig->SetPath("/Controls");
		///*write user defined values to the config file*/
		//pConfig->Write("m_IP", hostname);
		//pConfig->Write("m_PORT", port);

		///*initiate adress ip and port by user defined values*/
		//addr->Hostname(hostname);
		//addr->Service(port);
	}

	//If there are already Ip and HOST values in the config file
	else
	{
		Config_flag = false;
		//setup config path
		pConfig->SetPath("/Controls");
		//and read those values
		hostname = pConfig->Read("m_IP");
		port = pConfig->Read("m_PORT");

		//and initiate the address by values from config file
		addr->Hostname(hostname);
		addr->Service(port);

		/////////////////////////////////////////////////////////////////////
		/// here we need to implement some function to delete existing config 
		/// by some keys combination
		/// /////////////////////////////////////////////////////////////////
	}
}

IpPanel::~IpPanel()
{
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
	//wxUnusedVar(family); // unused in !wxUSE_IPV6 case

	Communicate* comm = (Communicate*)m_parent->GetParent();
	wxString m_IP;
	wxString m_PORT;

	//Define IP adrees
	wxIPaddress* addr;
	// and set it to IP adress type 4
	wxIPV4address addr4;
	addr = &addr4;

	//Open the config file
	wxConfigBase* pConfig = wxConfigBase::Get();
	if (pConfig == NULL)
		return;

	// and read the control's values to the config
	m_IP = pConfig->Read("/Controls/m_IP");
	m_PORT = pConfig->Read("/Controls/m_PORT");

	addr->Hostname(m_IP);
	addr->Service(m_PORT);
	//Save the IP and Host name to config file

	m_sock->Connect(*addr, false);
	// we connect asynchronously and will get a wxSOCKET_CONNECTION event when
	// the connection is really established

	// if you want to make sure that connection is established right here you
	// could call WaitOnConnect(timeout) instead
	//wxMessageBox("Trying to connect to %s:%s", addr->Hostname(), addr->Service());

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
	Communicate* comm = (Communicate*)m_parent->GetParent();
	if (comm->m_ip->Config_flag == true)
	{
		if (count_msg > 17)
		{
			count_msg = 17;
		}
		else count_msg++;
	}


	comm->m_ip->m_ip_1->SetFocus();

	comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_msg));
	//send text to different text boxes
	comm->m_mp->m_text_main->SetLabel(text_main);
	comm->m_midp->m_text_mid->SetLabel(text_main);
}


void MainPanel::OnMsgMinus(wxCommandEvent& WXUNUSED(event))
{
	Communicate* comm = (Communicate*)m_parent->GetParent();
	if (comm->m_ip->Config_flag == true)
	{
		if (count_msg < 0)
		{
			count_msg = 0;
		}
		else count_msg--;
	}
	text_main = ("MESSAGE MSG- RECEIVED");
	
	//send text to different text boxes
	comm->m_mp->m_text_main->SetLabel(text_main);
	comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_msg));
	comm->m_midp->m_text_mid->SetLabel(text_main);
}

void MainPanel::OnPOEPlus(wxCommandEvent& WXUNUSED(event))
{
	Communicate* comm = (Communicate*)m_parent->GetParent();
	if (comm->m_ip->Config_flag == true)
	{
		if (count_msg > 9)
		{
			count_msg = 9;
		}
		else count_msg++;
	}

	count_poe++;
	text_main = ("MESSAGE PGE+ RECEIVED");
	
	//send text to different text boxes
	comm->m_mp->m_text_main->SetLabel(text_main);
	comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_poe));
	comm->m_midp->m_text_mid->SetLabel(text_main);
}

void MainPanel::OnPOEMinus(wxCommandEvent& WXUNUSED(event))
{
	Communicate* comm = (Communicate*)m_parent->GetParent();
	if (comm->m_ip->Config_flag == true)
	{
		if (count_msg < 0)
		{
			count_msg = 0;
		}
		else count_msg++;
	}
	
	count_poe--;
	text_main = ("MESSAGE PGE- RECEIVED");
	
	//send text to different text boxes
	comm->m_mp->m_text_main->SetLabel(text_main);
	comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_poe));
	comm->m_midp->m_text_mid->SetLabel(text_main);
}



//RightPanel setup

RightPanel::RightPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) * 3 / 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) * 2 / 3),
		wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4, wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 3), /*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
	//font setup
	right_font = new wxFont(38, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");

	count_r = 0;
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

