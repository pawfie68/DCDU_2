#include <wx/stattext.h>
#include "Communicate.h"
#include "Colors_def.h"

#include "wx/config.h"


#include "Button_handlers.h"
//#include "Net.h"

//MainPanel scaling with the screen resolution

MainPanel::MainPanel(wxPanel* parent)
	: wxPanel(parent, -1, wxPoint(-1, -1), wxSize(X_screen,
		Y_screen * 2 / 3), /*wxBORDER_SUNKEN*/ wxNO_BORDER )
{
	
	//font setting
	main_font = new wxFont(font_size_main, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial" /*"New Font"*/);
	top_font = new wxFont(font_size_top, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial" /*"New Font"*/);
	//counters for handling number of clicks -> for future use
	//main_font->AddPrivateFont("H:\\Repos\\DCDU-master\\DCDU_REGULAR.ttf");
	//setting the parent for enabling communication between panels
	m_parent = parent;
	//display the standby message
	m_top_open = new wxStaticText(this, -1, wxT(""), wxPoint((X_screen * 3 / 4), 10), wxSize((X_screen * 1 / 4) - 10,
		(Y_screen / 10) - 20), /*wxBORDER_SUNKEN*/ /*wxNO_BORDER*/ wxALIGN_RIGHT);
	m_top_open->SetForegroundColour(wxColor(CYAN));
	m_top_open->SetBackgroundColour(wxColor(BLACK));
	m_top_open->SetFont(*main_font);
	//m_top_open->
	//m_top_open->do
	m_top_open->SetLabel("OPEN");
	//m_top_main->Hide();


	m_top_main = new wxStaticText(this, -1, wxT("Connecting..."), wxPoint(10, 10), wxSize((X_screen*3/4) - 20,
		(Y_screen/10)-20), wxBORDER_SUNKEN /*wxNO_BORDER*/ );
	m_top_main->SetForegroundColour(wxColor(GREEN));
	m_top_main->SetBackgroundColour(wxColor(BLACK));
	m_top_main->SetFont(*top_font);
	//m_top_main->Hide();

	m_rtext_main = new wxRichTextCtrl(this, -1, wxT(""), wxPoint(10, (Y_screen/10)-10), wxSize(X_screen-20,
		Y_screen* 9/10 * 2 / 3 - 20) , wxBORDER_SUNKEN /*wxNO_BORDER*/);
	m_rtext_main->SetFont(*main_font);

	m_rtext_main->BeginTextColour(wxColor(WHITE));
	m_rtext_main->SetBackgroundColour(wxColor(BLACK));
	m_rtext_main->ForceUpper();
	m_rtext_main->DiscardEdits();
	m_rtext_main->GetVisibleLineForCaretPosition(true);
	m_rtext_main->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);
	m_rtext_main->SetScrollPageSize(((X_screen * 3 / 4) - 20), (Y_screen / 10) - 20);
	m_rtext_main->Enable(false);
	m_rtext_main->Hide();

	//creating button for MSG+ and connect it to the proper event
	msg_plus = new wxButton(this, ID_MSG_PLUS, wxT("MSG+"),
		wxPoint(10, Y_screen * 1 / 3 + 100));
	msg_plus->SetBackgroundColour(wxColor(110, 110, 110));
	msg_plus->SetForegroundColour(wxColor(*wxWHITE));
	Connect(ID_MSG_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(MainPanel::OnMsgPlus));

	//creating button for MSG- and connect it to the proper event
	msg_minus = new wxButton(this, ID_MSG_MINUS, wxT("MSG-"),
		wxPoint(10, Y_screen * 1 / 3 + 50));
	msg_minus->SetBackgroundColour(wxColor(110, 110, 110));
	msg_minus->SetForegroundColour(wxColor(*wxWHITE));
	Connect(ID_MSG_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(MainPanel::OnMsgMinus));

	//creating button for POE+ and connect it to the proper event
m_POE_plus = new wxButton(this, ID_POE_PLUS, wxT("PGE +"),
	wxPoint(X_screen - 100, Y_screen * 1 / 3 + 100));
m_POE_plus->SetBackgroundColour(wxColor(110, 110, 110));
m_POE_plus->SetForegroundColour(wxColor(*wxWHITE));
Connect(ID_POE_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
	wxCommandEventHandler(MainPanel::OnPOEPlus));

//creating button for POE+ and connect it to the proper event
m_POE_minus = new wxButton(this, ID_POE_MINUS, wxT("PGE -"),
	wxPoint(X_screen - 100, Y_screen * 1 / 3 + 50));
m_POE_minus->SetBackgroundColour(wxColor(110, 110, 110));
m_POE_minus->SetForegroundColour(wxColor(*wxWHITE));
Connect(ID_POE_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
	wxCommandEventHandler(MainPanel::OnPOEMinus));


//creating button for POE+ and connect it to the proper event
m_print = new wxButton(this, ID_PRINT, wxT("PRINT"),
	wxPoint(X_screen - 100, 50));
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
	comm->m_ip->IpRefresh();
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

void IpPanel::IpRefresh()
{
	// we want to set focus on all elements of IP panel one by one, to display them on the front of the main app window
	SetFocus();
	m_ip_2->SetFocus();
	m_ip_3->SetFocus(); m_ip_3->SetFocus();
	m_ip_5->SetFocus(); m_ip_6->SetFocus();
	m_ip_7->SetFocus(); m_ip_8->SetFocus();
	m_ip_9->SetFocus(); m_ip_10->SetFocus();
	m_ip_11->SetFocus(); m_ip_12->SetFocus();
	m_ip_13->SetFocus(); m_ip_14->SetFocus();
	m_ip_15->SetFocus(); m_ip_16->SetFocus();
	m_ip_17->SetFocus(); m_ip_1->SetFocus();
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



void IpPanel::NetConfig()
{
	/* communicate class to handle data transfer between the panels */
	//Communicate* comm = (Communicate*)m_parent->GetParent();

	bool e_read_cfg_ip = false;
	bool e_read_cfg_port = false;

	//zero base indexes of dots in IP adress from config
	int first_dot_i = 0, second_dot_i = 0, third_dot_i = 0;

	//some int variable to define difference in size between string readed from cfg file
	//to size of buffer [50] char, fill with ' ' sign
	int lengthen_cfg_str;
	lengthen_cfg_str = wxStrlen(ip_from_cfg);

	for (int inee = lengthen_cfg_str; inee <= 20; inee++)
	{
		ip_from_cfg.append(' ');

	}

	//to be sure not to work on empty string

	if (ip_from_cfg.Length() > 0)
	{
		// some local iterators
		// 'i' index is used to iterate over IP string readed from config file
		// 'j' index is the index of complete ip adress to display in form of ___.___.___.___

		int i = 0, j = 0, dot_count = 0;

		//main loop to fill all necessery spaces in the buffer with proper signs from cfg file
		for (wxString::const_iterator it = ip_from_cfg.begin(); it != ip_from_cfg.end() - 5; it++)
		{
			//if dot is present two places earlier tha should, we would like to make sure this doesnt hapend at any point of IP adress
			//in if condition we specify index of 'i' at which such situation could take place
			//then we chech if the dot is present, and
			if ((i <= 1 && ip_from_cfg[i] == '.') ||
				(i > 2 && i <= 5 && (ip_from_cfg[i] == '.')) && j == 5 && dot_count == 1 ||
				(i > 4 && i <= 9 && (ip_from_cfg[i] == '.')) && j == 9 && dot_count == 2)
			{
				//dot_count told us how many dots are present in the buffer
				dot_count++;

				//add null value to the current index
				ip_to_display[j] = NULL;
				//set null value at second index 
				ip_to_display[j + 1] = NULL;
				//and then place the dot in proper place
				ip_to_display[j + 2] = ip_from_cfg[i];

				//and set all indexers at correct value;
				j += 3;
				i++;

			}
			//check if the '.' is present in the IP one place erlier than should
			else if ((i > 1 && i <= 2 && ip_from_cfg[i] == '.') ||
				(i > 2 && i <= 6 && ip_from_cfg[i] == '.') && dot_count == 1 && j == 6 ||
				(i > 6 && i <= 10 && ip_from_cfg[i] == '.' && dot_count == 2) && j == 10)
			{
				//dot_count told us how many dots are present in the buffer
				dot_count++;
				//set value at current index as null 
				ip_to_display[j] = NULL;
				//and then place the dot in place
				ip_to_display[j + 1] = ip_from_cfg[i];

				//and set all indexers at correct value;

				j += 2;
				i++;

			}
			//Then change empty spaces added before to NULL values
			else if (ip_from_cfg[i] == ' ')
			{
				ip_to_display[j] = NULL;

				//and shift indexes
				i++;
				j++;
			}

			//In normal case, just place values from cfg file in place
			else
			{
				ip_to_display[j] = ip_from_cfg[i];
				if (ip_from_cfg[i] == '.')
				{
					dot_count++;
				}
				//and ofc shift the iterators
				i++;
				j++;
			}
		}

	}//and error to handle if case if sth goes wrong
	else e_read_cfg_ip = true;

	//And the port number from config need to be placed in proper char array
	//but firstly make sure that the cfg string is long enough

	int lengthen_cfg_str_port;
	lengthen_cfg_str_port = wxStrlen(port_from_cfg);


	for (int i = lengthen_cfg_str_port; i <= 7; i++)
	{
		port_from_cfg.append(' ');
	}

	int i = 0;


	for (wxString::const_iterator it = port_from_cfg.begin(); it != port_from_cfg.end(); it++)
	{

		if (port_from_cfg[i] == ' ')
		{
			port_to_dispaly[i] = NULL;

		}
		else
		{
			port_to_dispaly[i] = port_from_cfg[i];
		}
		i++;
	}

	//After all values from string are loaded to the correct places in the char array, we can also update the values used as a iterators
	//on button events to increment or decrement numbers in IP adresses
	//In theory the char aray members can be use to that purpose, but in that case we need additional values to store the index of
	//IP digit we would like to modify, so for now ill stay with the solution where the iterators are stored in separate chars.
	//this modifier value is needed to skip dots in the IP adress when transfering them to the counters. 
	//the ip_to_display caounter is [i - 1] becaouse count_poe is a 1 base index
	int some_modifier = 0;
	for (int i = 1; i < 21; i++)
	{
		if (i <= 15)
		{
			if (ip_to_display[i - 1] != '.' && ip_to_display[i - 1] != 0)
				count_poe[i - some_modifier] = ip_to_display[i - 1] - 48;
			else if (ip_to_display[i - 1] == 0)
				count_poe[i - some_modifier] = ip_to_display[i - 1];
			else some_modifier++;
		}
		else
		{
			count_poe[i - some_modifier] = port_to_dispaly[i - 16] - 48;
		}

	}
}



void MainPanel::OnSocketEvent(wxSocketEvent& event)
{
	Communicate* comm = (Communicate*)m_parent->GetParent();
	//wxBuffer to collect data from socket with 16 black spaces
	//Propably we will need only 1 sign at the time, but its larger for future use
	wxCharBuffer buf2 = "                 ";
	wxString s_top;
	wxString temp_adress;
	wxString temp_port;
	//here we will handle the socket events 
	switch (event.GetSocketEvent())
	{
		//when data arrived
	case wxSOCKET_INPUT:

		//reading 15 characters (1 should be enough, but to be safe, larger buffor was applied)
		//and save them to the buffer buf2 || if sended data is shorter, the read function will
		//collect only available data  
		m_sock->Read(buf2.data(), 15);
		Received.append(buf2.data());
		Received.Trim();

		m_rtext_main->BeginTextColour(*wxWHITE);

		if (buf2.data()[0] == 'a')
		{
			m_rtext_main->BeginTextColour(wxColor(AMBER));
			Received[0] = NULL;
		}
		else if (buf2.data()[0] == 'c')
		{
			m_rtext_main->BeginTextColour(wxColor(CYAN));
			Received[0] = NULL;
		}
		else if (buf2.data()[0] == 'g')
		{
			m_rtext_main->BeginTextColour(wxColor(GREEN));
			Received[0] = NULL;
		}
		else if (buf2.data()[0] == 'm')
		{
			m_rtext_main->BeginTextColour(wxColor(MAGENTA));
			Received[0] = NULL;
		}
		else if (buf2.data()[0] == 'r')
		{
			m_rtext_main->BeginTextColour(wxColor(RED));
			Received[0] = NULL;
		}
		else if (buf2.data()[0] == 'w')
		{
			m_rtext_main->BeginTextColour(wxColor(WHITE));
			Received[0] = NULL;
		}
		else if (buf2.data()[0] == 'y')
		{
			m_rtext_main->BeginTextColour(wxColor(YELLOW));
			Received[0] = NULL;
		}
		
		Received = Received.Upper();

		//Received[0] = buf2.data();
		//write back data from buf2, 
		
		m_sock->Write(Received, Received.length());
		m_rtext_main->Scroll(0, m_rtext_main->GetVirtualSize().y - m_rtext_main->GetClientSize().y);
		//m_sock->Write(buf2.data(), wxStrlen(buf2.data()));

		//and display them on the scren, clean screen first
		//m_rtext_main->Clear();
		//m_text_main->SetLabel(buf2.data());
		m_rtext_main->WriteText(Received);
		//m_rtext_main->AddParagraph(buf2.data());
		//m_rtext_main->WriteText(buf2.data());
		//m_rtext_main->SetLabel(Received);
		
		int test;
		test = Received.length();

		s_top.append("RECEIVED :");
		s_top.append(wxString::Format(wxT("%ld"), test));
		s_top.append(" BYTES");
	

		Received = "";
		m_top_main->SetLabel(s_top);
		m_top_main->Show();
		break;

		//when we lost connection with server
	case wxSOCKET_LOST:

		//display the inflo about the event
		comm->m_midp->m_text_mid->SetForegroundColour(wxColor(255, 0, 0));
		
		comm->m_midp->m_text_mid->SetLabel("SOCKET CON ERROR");
		comm->m_midp->m_text_mid->SetForegroundColour(wxColor(0, 255, 0));
		Disconnect();
		m_rtext_main->Hide();
		m_top_main->Hide();
		m_top_open->Hide();
		comm->m_ip->Show();
		comm->m_ip->IpRefresh();
		break;

		//when we establish connection with server
	case wxSOCKET_CONNECTION:

		//display the inflo about the connection
		
		comm->m_ip->Hide();
		m_rtext_main->Show();
		m_top_main->Show();
		m_top_open->Show();
		m_top_main->SetLabel("CONNECTED TO SERVER");
		break;

		//Other events will trigger this action
	default:
		
		m_top_main->SetLabel("UNKNOWN SOCKET EVENT!!!");
		
		m_rtext_main->Hide();
		m_top_main->Hide();
		m_top_open->Hide();
		comm->m_ip->Show();
		comm->m_ip->IpRefresh();
		break;
	}
}

void MainPanel::OnOpenConnection(wxCommandEvent& WXUNUSED(event))
{
	//This function will be handle later when physical buttons wil be in place
	// OpenConnection(wxSockAddress::IPV4);
}

bool MainPanel::OpenConnection(wxSockAddress::Family family)
{
	Communicate* comm = (Communicate*)m_parent->GetParent();

	//Here we define strings to store the values readed from config file
	wxString m_IP;
	wxString m_PORT;

	//Define IP adrees
	wxIPaddress* addr;
	// and set it to be IP adress type 4
	wxIPV4address addr4;
	addr = &addr4;

	//Open the config file
	wxConfigBase* pConfig = wxConfigBase::Get();
	//when there is no such file - return 
	if (pConfig == NULL)
		return false;

	// and read the control's values to the config
	m_IP = pConfig->Read("/Controls/m_IP");
	m_PORT = pConfig->Read("/Controls/m_PORT");


	//initialize the adress with IP and PORT from config file
	addr->Hostname(m_IP);
	addr->Service(m_PORT);

	//And finally connect to the server

	m_sock->Connect(*addr, false);

	// we connect asynchronously and will get a wxSOCKET_CONNECTION event when
	// the connection is really established

	// if you want to make sure that connection is established right here you
	// could call WaitOnConnect(timeout) instead

	//We can check if the connection is established
	if (m_sock->IsConnected())
	{
		//and inform the user about it

		m_rtext_main->SetLabel("connected");
		return true;
	}
	else
	{
		//or when we cannot connect with the provided adress, then display the error
		m_rtext_main->SetLabel("connection error");
		comm->m_ip->Config_flag = true;
		return false;
	}
	m_busy = false;

}

void MainPanel::Disconnect()
{
	Communicate* comm = (Communicate*)m_parent->GetParent();
	if (m_sock->IsConnected())
	{
		m_sock->Close();
		comm->m_midp->m_text_mid->SetLabel("Disconnected succesfully");
	}
	else
	{
		comm->m_midp->m_text_mid->SetLabel("No connection found");

	}

}


//======================================================================================================================================
// In this part we will deal with the constructors of the auxilary panels and events connected to them
//======================================================================================================================================
// 
//RightPanel setup
//As a condition we provide the size of the panel in relation to the screen resolution, and relative point at which we will start to draw

RightPanel::RightPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxPoint(X_screen * 3 / 4 , Y_screen * 2 / 3),
		wxSize(X_screen / 4, Y_screen / 3), /*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
	//font setup
	right_font = new wxFont(38, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	count_r = 0; // this counter will be removed soon
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//text setup
	m_text = new wxStaticText(this, -1, wxT("0000000000000"), wxPoint((X_screen * 4 / 5) / 2, 10));
	m_text_r1 = new wxStaticText(this, -1, wxT("MESSAGE#"), wxPoint(10, 30), wxSize(X_screen / 4 - X_screen/40, Y_screen / 30 - 50), wxALIGN_RIGHT);
	m_text_r1->SetFont(*right_font);
	m_text_r1->SetBackgroundColour(wxColor(BLACK));
	m_text_r1->SetForegroundColour(wxColor(CYAN));

	m_text_r2 = new wxStaticText(this, -1, wxT("WILCO#"), wxPoint(10, Y_screen/4 -50), wxSize(X_screen / 4 - X_screen / 40, Y_screen / 30 - 50), wxALIGN_RIGHT);
	m_text_r2->SetFont(*right_font);

	m_text_r2->SetBackgroundColour(wxColor(BLACK));
	m_text_r2->SetForegroundColour(wxColor(CYAN));
	m_text->SetForegroundColour(wxColor(CYAN));

	//creating button for RightUP and connect it to the proper event
	m_right_up = new wxButton(this, ID_RU, wxT("R UP"),
		wxPoint(X_screen / 4 - 100
			, Y_screen / 3 - 150));
	m_right_up->SetBackgroundColour(wxColor(110, 110, 110));
	m_right_up->SetForegroundColour(wxColor(*wxWHITE));
	Connect(ID_RU, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(RightPanel::OnRightUp));

	//creating button for RightDWN and connect it to the proper event
	m_right_down = new wxButton(this, ID_RD, wxT("R DWN"),
		wxPoint(X_screen / 4 - 100,
			Y_screen / 3 - 100));
	m_right_down->SetBackgroundColour(wxColor(110, 110, 110));
	m_right_down->SetForegroundColour(wxColor(*wxWHITE));
	Connect(ID_RD, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(RightPanel::OnRightDown));
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	m_IP_value = 0; //the name of this counters will be changed
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}


LeftPanel::LeftPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxPoint(0, Y_screen * 2 / 3),
		wxSize(X_screen / 4, Y_screen / 3),
		/*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
	left_font = new wxFont(38, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");
	count_l = 0;

	//m_text_r1 = new wxStaticText(this, -1, wxT("MESSAGE#"), wxPoint(10, 30), wxSize(X_screen / 4 - X_screen / 40, Y_screen / 30 - 50), wxALIGN_RIGHT);
	//m_text_r2 = new wxStaticText(this, -1, wxT("WILCO#"), wxPoint(10, Y_screen / 4 - 50), wxSize(X_screen / 4 - X_screen / 40, Y_screen / 30 - 50), wxALIGN_RIGHT);


	m_text_l1 = new wxStaticText(this, -1, wxT("#UNABLE"), wxPoint(X_screen / 40, 30), wxSize(X_screen / 4 + X_screen / 40, Y_screen / 30 - 50), wxALIGN_LEFT);
	m_text_l1->SetFont(*left_font);
	m_text_l1->SetForegroundColour(wxColor(0, 255, 255));;

	m_text_l2 = new wxStaticText(this, -1, wxT("<OTHER"), wxPoint(X_screen / 40, Y_screen / 4 - 50), wxSize(X_screen / 4 + X_screen / 40, Y_screen / 30 - 50), wxALIGN_LEFT);
	m_text_l2->SetFont(*left_font);
	m_text_l2->SetForegroundColour(wxColor(0, 255, 255));;


	//creating button for LeftUP and connect it to the proper event
	m_left_up = new wxButton(this, ID_LU, wxT("L UP"),
		wxPoint(10, Y_screen / 3 - 150));
	Connect(ID_LU, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(LeftPanel::OnLeftUp));
	m_left_up->SetBackgroundColour(wxColor(110, 110, 110));
	m_left_up->SetForegroundColour(wxColor(*wxWHITE));
	//creating button for LeftUP and connect it to the proper event
	m_left_down = new wxButton(this, ID_LD, wxT("L DWN"),
		wxPoint(10, Y_screen / 3 - 100));
	Connect(ID_LD, wxEVT_COMMAND_BUTTON_CLICKED,
		wxCommandEventHandler(LeftPanel::OnLeftDown));
	m_left_down->SetBackgroundColour(wxColor(110, 110, 110));
	m_left_down->SetForegroundColour(wxColor(*wxWHITE));

	m_IP_index = 0;
}


//middle panel will be modified only by other events, doesnt have any event by itself

MidlePanel::MidlePanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxPoint(X_screen / 4, Y_screen * 2 / 3),
		wxSize(X_screen / 2, Y_screen * 2 / 3),
		/*wxBORDER_SUNKEN*/  wxNO_BORDER)
{
	//font setup
	mid_font = new wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN, false, "Arial Rounded");
	text_mid = "BUTTON CLICKED: ";

	//textbox for midle screen
	m_text_mid = new wxStaticText(this, -1, wxT("MESSAGE FOR MID SCREEN\nWILL APPEAR HERE"), wxPoint(10, 10),
		wxSize(X_screen / 2 - 20, Y_screen / 3 - 20),
		wxEXPAND | wxALIGN_CENTER);

	m_text_mid->SetFont(*mid_font);

	m_text_mid->SetForegroundColour(wxColor(52, 235, 161));;

	count_m = 0;
}


//additional screen -> will display only page counter in the future
//adjustment of size will be needed on final device

PgCounterPanel::PgCounterPanel(wxPanel* parent)
	: wxPanel(parent, wxID_ANY, wxPoint(X_screen * 3 / 4 - 250, Y_screen - 100),
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


//======================================================================================================================================
//Events on buttons in main panel//
//======================================================================================================================================

void MainPanel::OnPrintEvent(wxCommandEvent& WXUNUSED(event))
{
	/* communicate class to handle data transfer between the panels */
	Communicate* comm = (Communicate*)m_parent->GetParent();
	comm->m_mp->m_rtext_main->SetLabel("");
	if (m_sock->IsConnected())
	{
		Disconnect();
		comm->m_ip->Config_flag = true;
		comm->m_ip->Show(true);

	}
	else if (comm->m_ip->Config_flag)
	{
		//For now Print button is used only deal with the data supplied by the user,
		wxString ip_from_user;
		wxString port_from_user;
		//wxIPV4address adr;
		wxConfigBase* pConfig = wxConfigBase::Get();

		comm->m_ip->Show(true);

		addr = &addr4;


		//after PRINT button clicked, check every part of IP adrees, if empty get back to typing
		if ((comm->m_ip->m_ip_1->GetValue().Length() + comm->m_ip->m_ip_2->GetValue().Length() + comm->m_ip->m_ip_3->GetValue().Length()) == 0 ||
			(comm->m_ip->m_ip_4->GetValue().Length() + comm->m_ip->m_ip_5->GetValue().Length() + comm->m_ip->m_ip_6->GetValue().Length()) == 0 ||
			(comm->m_ip->m_ip_7->GetValue().Length() + comm->m_ip->m_ip_8->GetValue().Length() + comm->m_ip->m_ip_9->GetValue().Length()) == 0 ||
			(comm->m_ip->m_ip_10->GetValue().Length() + comm->m_ip->m_ip_11->GetValue().Length() + comm->m_ip->m_ip_12->GetValue().Length()) == 0 ||
			(comm->m_ip->m_ip_13->GetValue().Length() + comm->m_ip->m_ip_14->GetValue().Length() + comm->m_ip->m_ip_15->GetValue().Length() +
				comm->m_ip->m_ip_16->GetValue().Length() + comm->m_ip->m_ip_17->GetValue().Length() == 0))
		{
			comm->m_midp->m_text_mid->SetLabel("Typed IPv4 incorrect");
		}
		else
		{
			//when all parts of ip adress contain data we can create IP and Host name from single leters 
			ip_from_user = comm->m_ip->m_ip_1->GetValue() + comm->m_ip->m_ip_2->GetValue() + comm->m_ip->m_ip_3->GetValue() + "."
				+ comm->m_ip->m_ip_4->GetValue() + comm->m_ip->m_ip_5->GetValue() + comm->m_ip->m_ip_6->GetValue() + "."
				+ comm->m_ip->m_ip_7->GetValue() + comm->m_ip->m_ip_8->GetValue() + comm->m_ip->m_ip_9->GetValue() + "."
				+ comm->m_ip->m_ip_10->GetValue() + comm->m_ip->m_ip_11->GetValue() + comm->m_ip->m_ip_12->GetValue();
			port_from_user = comm->m_ip->m_ip_13->GetValue() + comm->m_ip->m_ip_14->GetValue() + comm->m_ip->m_ip_15->GetValue()
				+ comm->m_ip->m_ip_16->GetValue() + comm->m_ip->m_ip_17->GetValue();

			//and save them to the config file
			pConfig->Write("/Controls/m_IP", ip_from_user);
			pConfig->Write("/Controls/m_PORT", port_from_user);

			//after we saved the data, double check if they are the same as the value in config file,
			//only then we can send the values to the IpPanel, open conection, and then destroy the IpPanel
			if (pConfig->Read("/Controls/m_IP") == ip_from_user && pConfig->Read("/Controls/m_PORT") == port_from_user)
			{
				comm->m_midp->m_text_mid->SetLabel("Adress saved succesfully");
				wxMessageBox("test");
				comm->m_ip->hostname = ip_from_user;
				comm->m_ip->port = port_from_user;
				//comm->m_ip->Config_flag = false;

				if (OpenConnection(wxSockAddress::IPV4))
				{
					//	comm->m_ip->Hide();
				}
				else
				{
					//	 m_text_main->SetLabel("d");
				}

			}
		}
	}
	else
	{
		m_sock->Close();
		if (m_sock->IsDisconnected())
		{
			comm->m_ip->Config_flag = true;
			comm->m_midp->m_text_mid->SetLabel("Disconnected");
			comm->m_ip->Show(true);

			//comm->m_ip->m_ip_1->SetFocus();
			//comm->m_mp->Hide();
		}
	}
}

void MainPanel::OnMsgPlus(wxCommandEvent& WXUNUSED(event))
{
	Communicate* comm = (Communicate*)m_parent->GetParent();


	if (comm->m_ip->Config_flag == true)
	{
		//	comm->m_ip->count_msg = 5;
		if (comm->m_ip->count_msg >= 17)
		{

			comm->m_ip->count_msg = 17;
			ButtonClicked();
		}
		else
		{

			comm->m_ip->count_msg++;
			ButtonClicked();
		}
	}
	else
	{
		//comm->m_ip->m_ip_1->SetFocus();

		comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), comm->m_ip->count_msg));
		//send text to different text boxes
		comm->m_mp->m_rtext_main->SetLabel(text_main);
		comm->m_midp->m_text_mid->SetLabel(text_main);
	}
}


void MainPanel::OnMsgMinus(wxCommandEvent& WXUNUSED(event))
{
	Communicate* comm = (Communicate*)m_parent->GetParent();
	if (comm->m_ip->Config_flag == true)
	{

		if (comm->m_ip->count_msg <= 1)
		{
			comm->m_ip->count_msg = 1;
			ButtonClicked();
		}
		else
		{
			comm->m_ip->count_msg--;
			ButtonClicked();
		}

	}
	else
	{
		text_main = ("MESSAGE MSG- RECEIVED");

		//send text to different text boxes
		comm->m_mp->m_rtext_main->SetLabel(text_main);
		comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), comm->m_ip->count_msg));
		comm->m_midp->m_text_mid->SetLabel(text_main);
	}
}

void MainPanel::OnPOEPlus(wxCommandEvent& WXUNUSED(event))
{
	Communicate* comm = (Communicate*)m_parent->GetParent();
	if (comm->m_ip->Config_flag == true)
	{

		if (comm->m_ip->count_poe[comm->m_ip->count_msg] >= 9)
		{
			comm->m_ip->count_poe[comm->m_ip->count_msg] = 9;
			ButtonClicked();
		}
		else
		{
			comm->m_ip->count_poe[comm->m_ip->count_msg]++;
			ButtonClicked();
		}

	}
	else
	{
		text_main = ("MESSAGE PGE+ RECEIVED");

		//send text to different text boxes
		comm->m_mp->m_rtext_main->SetLabel(text_main);
		comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), comm->m_ip->count_poe));
		comm->m_midp->m_text_mid->SetLabel(text_main);
	}
}

void MainPanel::OnPOEMinus(wxCommandEvent& WXUNUSED(event))
{
	Communicate* comm = (Communicate*)m_parent->GetParent();
	if (comm->m_ip->Config_flag == true)
	{
		if (comm->m_ip->count_poe[comm->m_ip->count_msg] <= -1)
		{
			comm->m_ip->count_poe[comm->m_ip->count_msg] = -1;
			ButtonClicked();
		}
		else
		{
			comm->m_ip->count_poe[comm->m_ip->count_msg]--;
			ButtonClicked();
		}
	}
	else
	{
		//count_poe--;
		text_main = ("MESSAGE PGE- RECEIVED");

		//send text to different text boxes
		comm->m_mp->m_rtext_main->SetLabel(text_main);
		comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), comm->m_ip->count_poe));
		comm->m_midp->m_text_mid->SetLabel(text_main);
	}
}


//======================================================================================================================================
//Events on buttons in right bottom panel//
//======================================================================================================================================

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
	comm->m_rp->m_text_r1->SetLabel(text_r);
	comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_r));
	comm->m_midp->m_text_mid->SetLabel(text_r);


	//	if (wxGetKeyState(WXK_F1)) comm->m_mp->OnMultiButtons();


}

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
	comm->m_rp->m_text_r1->SetLabel(text_r);
	comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_r));
	comm->m_midp->m_text_mid->SetLabel(text_r);
}



//======================================================================================================================================
//Events on buttons in left bottom panel//
//======================================================================================================================================

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
	comm->m_lp->m_text_l1->SetLabel(text_l);
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
	comm->m_lp->m_text_l1->SetLabel(text_l);
	comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), count_l));
	comm->m_midp->m_text_mid->SetLabel(text_l);
}

//======================================================================================================================================
//This finction is called only when the config flag is false
//Only buttons POE and MSG will call this function
//The main purpose of ButtonClicked() is to switch between the IP UI windows and modify values stored in them
//======================================================================================================================================

void MainPanel::ButtonClicked()
{
	Communicate* comm = (Communicate*)m_parent->GetParent();

	if (comm->m_ip->Config_flag == true)
	{

		switch (comm->m_ip->count_msg)
		{

		case 1:
			comm->m_ip->m_ip_1->SetFocus();

			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_1->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_1->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}

			break;

		case 2:

			comm->m_ip->m_ip_2->SetFocus();

			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_2->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_2->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

		case 3:

			comm->m_ip->m_ip_3->SetFocus();

			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_3->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_3->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

		case 4:
			comm->m_ip->m_ip_4->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_4->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_4->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 5:
			comm->m_ip->m_ip_5->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_5->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_5->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 6:
			comm->m_ip->m_ip_6->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_6->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_6->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 7:
			comm->m_ip->m_ip_7->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_7->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_7->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 8:
			comm->m_ip->m_ip_8->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_8->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_8->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 9:
			comm->m_ip->m_ip_9->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_9->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_9->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 10:
			comm->m_ip->m_ip_10->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_10->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_10->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 11:
			comm->m_ip->m_ip_11->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_11->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_11->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 12:
			comm->m_ip->m_ip_12->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_12->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_12->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 13:
			comm->m_ip->m_ip_13->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_13->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_13->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 14:
			comm->m_ip->m_ip_14->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_14->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_14->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 15:
			comm->m_ip->m_ip_15->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_15->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_15->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 16:
			comm->m_ip->m_ip_16->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_16->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_16->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		case 17:
			comm->m_ip->m_ip_17->SetFocus();
			if (comm->m_ip->count_poe[comm->m_ip->count_msg] == -1)
			{
				comm->m_ip->m_ip_17->SetValue("");
			}
			else
			{
				comm->m_ip->m_ip_17->SetValue(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
			}
			break;

			break;
		}
	}
}