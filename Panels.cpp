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

	//setting the parent for enabling communication between panels
	m_parent = parent;
	//display the standby message
	m_text_main = new wxStaticText(this, -1, wxT(""), wxPoint(10, 20));
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


//creating button for POE+ and connect it to the proper event
m_print = new wxButton(this, ID_PRINT, wxT("PRINT"),
	wxPoint(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) - 100, 50));
m_print->SetBackgroundColour(wxColor(110, 110, 110));
m_print->SetForegroundColour(wxColor(*wxWHITE));
Connect(ID_PRINT, wxEVT_COMMAND_BUTTON_CLICKED,
	wxCommandEventHandler(MainPanel::OnPrintEvent));

Connect(ID_COMBINED, wxEVT_COMMAND_BUTTON_CLICKED,
	wxCommandEventHandler(MainPanel::OnPrintEvent));
//ID_COMBINED


//Bind(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainPanel::OnFourButtons), );

//Bind(wxEVT_COMMAND_BUTTON_CLICKED,
//	wxCommandEventHandler(MainPanel::OnButtonEvent), this);
//Connect(ID_MSG_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
//	wxCommandEventHandler(MainPanel::OnButtonEvent));
//Connect(ID_MSG_MINUS, wxEVT_COMMAND_BUTTON_CLICKED,
//	wxCommandEventHandler(MainPanel::OnButtonEvent));
//Connect(ID_MSG_PLUS, wxEVT_COMMAND_BUTTON_CLICKED,
//	wxCommandEventHandler(MainPanel::OnButtonEvent));

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
	
	OpenConnection(wxSockAddress::IPV4);
	//connect to IPV4 type adress 
	//OpenConnection(wxSockAddress::IPV4, addr);
}
else
{
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
/// the config file are corrupted ---------------------------------------------------------------------- ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////

IpPanel::IpPanel(wxPanel* parent)
	: wxPanel(parent, -1, wxPoint((wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2 - wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4), -1),
		wxSize(wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 2,
			wxSystemSettings::GetMetric(wxSYS_SCREEN_Y) / 5), /*wxBORDER_SUNKEN*/  wxSTAY_ON_TOP)
{
	Communicate* comm = (Communicate*)m_parent->GetParent();
	wxString ip_from_cfg;
	wxString port_from_cfg;
	wxConfigBase* pConfig = wxConfigBase::Get();

	wxIPaddress* addr;
	wxIPV4address addr4;
	addr = &addr4;


	ip_from_cfg = pConfig->Read("/Controls/m_IP");
	port_from_cfg = pConfig->Read("/Controls/m_PORT");
	hostname = ip_from_cfg;
	port = port_from_cfg;
	wxMessageBox(ip_from_cfg, port_from_cfg);

	if (ip_from_cfg.IsEmpty() ||
		port_from_cfg.IsEmpty())
	{
		Config_flag = true;
		wxMessageBox("config flag true");

	}

	//If there are already Ip and HOST values in the config file
	else
	{
		//////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!?////////////////
		///!!!HERE SWITCH ON CONNECTION BY SETTING CONFIG_FLAG false!!!!!!!#!##!!#####################################################
		/// 
		/// 
		
		//comm->m_mp->m_adress(hostname) = ip_from_user;
		addr->Hostname(port_from_cfg);
		addr->Service(port_from_cfg);
		//comm->m_ip->Config_flag = false;

		//coppy current address to the main panel frame
		/*comm->m_mp->OpenConnection(wxSockAddress::IPV4);*/

		Config_flag = false;

		wxMessageBox("config flag false");

		//setup config path
		//pConfig->SetPath("/Controls");
		//and read those values
		//hostname = pConfig->Read("m_IP");
		//port = pConfig->Read("m_PORT");

		//and initiate the address by values from config file
		//addr->Hostname(hostname);
		//addr->Service(port);

		/////////////////////////////////////////////////////////////////////
		/// here we need to implement some function to delete existing config 
		/// by some keys combination
		/// /////////////////////////////////////////////////////////////////
		//comm->m_mp->OpenConnection(wxSockAddress::IPV4);
		//~IpPanel();
	}

	/// ========================================================================================================================================
	/// When config is needed Config_flag == true and we run the proper functions
	/// else, do nothing
	

	if (Config_flag)
	{



		//wxChar ip_to_display[50];
		//wxChar port_to_dispaly[8];

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
			//lengthen_cfg_str = wxStrlen(ip_from_cfg);

		}

		//to be sure not to work on empty string

		if (ip_from_cfg.Length() > 0)
		{
			// some local iterators
			// 'i' index is used to iterate over IP string readed from config file
			// 'j' index is the index of complete ip adress to display in form of ___.___.___.___
			//w
			int i = 0, j = 0, dot_count = 0;
			wxMessageBox(ip_from_cfg);
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

		//After all values from string are loaded to the correct places in the char array, we can also update the values used as a iterators
		//on button events to increment or decrement numbers in IP adresses
		//In theory the char aray members can be use to that purpose, but in that case we need additional values to store the index of
		//IP digit we would like to modify, so for now ill stay with the solution where the iterators are stored in separate chars.
		//this modifier value is needed to skip dots in the IP adress when transfering them to the counters. 
		//the ip_to_display caounter is [i - 1] becaouse count_poe is a 1 base index
		int some_modifier = 0;
		for (int i = 1; i < 18; i++)
		{
			if (ip_to_display[i - 1] != '.' && ip_to_display[i - 1] != 0)
				count_poe[i - some_modifier] = ip_to_display[i - 1] - 48;
			else if (ip_to_display[i - 1] == 0)
				count_poe[i - some_modifier] = ip_to_display[i - 1];
			else some_modifier++;
			//else
			//	i--;
			//wxMessageBox(count_poe[i]);
		}


		int start_position = wxSystemSettings::GetMetric(wxSYS_SCREEN_X) / 4;
		wxString description = "<- [MSG-] / [MSG+] -> \n"  "+value [POE+] \n -value [POE-] \n"
			"sign empty under the 0\n"
			"[PRINT] - save IP/HOST";
		ip_font = new wxFont(30, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, "Arial");

		m_ip_1 = new wxTextCtrl(this, -1, ip_to_display[0], wxPoint(start_position - 330, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_1->SetFont(*ip_font);
		//count_poe[0] = int( ip_to_display[0]);
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

		m_ip_13 = new wxTextCtrl(this, -1, wxT(""), wxPoint(start_position + 175, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_13->SetFont(*ip_font);
		m_ip_13->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_13->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_14 = new wxTextCtrl(this, -1, wxT(""), wxPoint(start_position + 210, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_14->SetFont(*ip_font);
		m_ip_14->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_14->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_15 = new wxTextCtrl(this, -1, wxT(""), wxPoint(start_position + 245, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_15->SetFont(*ip_font);
		m_ip_15->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_15->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_16 = new wxTextCtrl(this, -1, wxT(""), wxPoint(start_position + 280, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_16->SetFont(*ip_font);
		m_ip_16->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_16->SetForegroundColour(wxColor(0, 0, 0));

		m_ip_17 = new wxTextCtrl(this, -1, wxT(""), wxPoint(start_position + 315, 20), wxSize(35, 50), wxNO_BORDER);
		m_ip_17->SetFont(*ip_font);
		m_ip_17->SetBackgroundColour(wxColor(0, 255, 0));
		m_ip_17->SetForegroundColour(wxColor(0, 0, 0));


		//Communicate* comm = (Communicate*)m_parent->GetParent();

		ip_setup_desc = new wxStaticText(this, -1, wxT(""), wxPoint(start_position - 500, 100), wxSize(400, 100), wxNO_BORDER);
		ip_setup_desc->SetFont(*ip_font);
		ip_setup_desc->SetBackgroundColour(wxColor(0, 0, 0));
		ip_setup_desc->SetForegroundColour(wxColor(0, 200, 100));

		ip_setup_desc->SetLabel(description);

		//comm->hbox->Add(m_ip_17);

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


		//comm->m_mp->addr = &addr4;
		//IP and PORT stored in registy->windows (/HKEY_CURRENT_USER/Software/YourVendor/YourApp)
		// propably /user/lib/local/Skalarki on unix (or in the app root location)
		// 

		/*Set up the config directory*/
		//  == = ========  pConfig->SetPath("/Controls");

		/*we need to check if there are already values stored in this location*/
		/*If they are empty, reed them from the user*/


		hostname = ip_to_display;
		port = port_to_dispaly;
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

void MainPanel::OnMultiButtons()
{
	wxMessageBox("sdasasd");
}

void MainPanel::OnPrintEvent(wxCommandEvent& WXUNUSED(event))
{
	wxString ip_from_user;
	wxString port_from_user;
	//wxIPV4address adr;

	wxConfigBase* pConfig = wxConfigBase::Get();

	addr = &addr4;

	/* communicate class to handle data transfer between the panels */
	Communicate* comm = (Communicate*)m_parent->GetParent();
	//after PRINT button clicked, check every part of IP adrees, if empty get back to typing
	int i = comm->m_ip->m_ip_1->GetValue().Length() + comm->m_ip->m_ip_2->GetValue().Length() + comm->m_ip->m_ip_3->GetValue().Length();
	wxMessageBox(wxString::Format(wxT("%ld"), i));
	if ((comm->m_ip->m_ip_1->GetValue().Length() + comm->m_ip->m_ip_2->GetValue().Length() + comm->m_ip->m_ip_3->GetValue().Length()) == 0 ||
		(comm->m_ip->m_ip_4->GetValue().Length() + comm->m_ip->m_ip_5->GetValue().Length() + comm->m_ip->m_ip_6->GetValue().Length()) == 0 ||
		(comm->m_ip->m_ip_7->GetValue().Length() + comm->m_ip->m_ip_8->GetValue().Length() + comm->m_ip->m_ip_9->GetValue().Length()) == 0 ||
		(comm->m_ip->m_ip_10->GetValue().Length() + comm->m_ip->m_ip_11->GetValue().Length() + comm->m_ip->m_ip_12->GetValue().Length()) == 0 ||
		(comm->m_ip->m_ip_13->GetValue().Length() + comm->m_ip->m_ip_14->GetValue().Length() + comm->m_ip->m_ip_15->GetValue().Length() +
		comm->m_ip->m_ip_16->GetValue().Length() + comm->m_ip->m_ip_17->GetValue().Length() == 0))
	{
		wxMessageBox("Typed IPv4 adrees or PORT contain empty elements, please provide valid adress");
	}
	else
	{
		ip_from_user = comm->m_ip->m_ip_1->GetValue() + comm->m_ip->m_ip_2->GetValue() + comm->m_ip->m_ip_3->GetValue() + "."
			+ comm->m_ip->m_ip_4->GetValue() + comm->m_ip->m_ip_5->GetValue() + comm->m_ip->m_ip_6->GetValue() + "."
			+ comm->m_ip->m_ip_7->GetValue() + comm->m_ip->m_ip_8->GetValue() + comm->m_ip->m_ip_9->GetValue() + "."
			+ comm->m_ip->m_ip_10->GetValue() + comm->m_ip->m_ip_11->GetValue() + comm->m_ip->m_ip_12->GetValue();
		port_from_user = comm->m_ip->m_ip_13->GetValue() + comm->m_ip->m_ip_14->GetValue() + comm->m_ip->m_ip_15->GetValue()
			+ comm->m_ip->m_ip_16->GetValue() + comm->m_ip->m_ip_17->GetValue();
		
		addr->Hostname(ip_from_user);
		addr->Service(port_from_user);

		pConfig->Write("/Controls/m_IP", ip_from_user);
		pConfig->Write("/Controls/m_PORT", port_from_user);

		if (pConfig->Read("/Controls/m_IP") == ip_from_user && pConfig->Read("/Controls/m_PORT") == port_from_user)
		{
			wxMessageBox("Values succesfully saved");
			comm->m_ip->hostname = ip_from_user;
			comm->m_ip->port = port_from_user;
			comm->m_ip->Config_flag = false;
			//comm->m_ip->~IpPanel();

			OpenConnection(wxSockAddress::IPV4);
			comm->m_ip->~IpPanel();
		}
		
	}
	
	
}


void MainPanel::OnSocketEvent(wxSocketEvent& event)
{
	//wxBuffer to collect data from socket

	wxCharBuffer buf2 = "               ";

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

	

	//connect to IPV4 type adress 
	//OpenConnection(wxSockAddress::IPV4);

	//wxUnusedVar(family); // unused in !wxUSE_IPV6 case

	//Communicate* comm = (Communicate*)m_parent->GetParent();
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
	wxMessageBox(m_IP);
	wxMessageBox(m_PORT);
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
		wxMessageBox("dupa");
	}
	else
	{
		m_text_main->SetLabelText(wxT("connection error"));
		wxMessageBox("dupa error");
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
		comm->m_mp->m_text_main->SetLabel(text_main);
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
		comm->m_mp->m_text_main->SetLabel(text_main);
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
			//wxMessageBox(wxString::Format(wxT("%i"), comm->m_ip->count_poe[comm->m_ip->count_msg]));
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
		//count_poe++;
		text_main = ("MESSAGE PGE+ RECEIVED");

		//send text to different text boxes
		comm->m_mp->m_text_main->SetLabel(text_main);
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
		comm->m_mp->m_text_main->SetLabel(text_main);
		comm->m_rp->m_text->SetLabel(wxString::Format(wxT("%d"), comm->m_ip->count_poe));
		comm->m_midp->m_text_mid->SetLabel(text_main);
	}

}



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

void MainPanel::OnButtonEvent(wxCommandEvent& WXUNUSED(event))
{



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

	
	if (wxGetKeyState(WXK_F1)) comm->m_mp->OnMultiButtons();
	

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

