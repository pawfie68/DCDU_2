#pragma once



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

		//Received[0] = buf2.data();
		//write back data from buf2, 

		m_sock->Write(Received, Received.length());
		//m_sock->Write(buf2.data(), wxStrlen(buf2.data()));

		//and display them on the scren, clean screen first
		m_text_main->SetLabel("");
		//m_text_main->SetLabel(buf2.data());


		m_text_main->SetLabel(Received);
		break;

		//when we lost connection with server
	case wxSOCKET_LOST:

		//display the inflo about the event
		m_text_main->SetLabel("Socket connection error");
		Disconnect();
		break;

		//when we establish connection with server
	case wxSOCKET_CONNECTION:

		//display the inflo about the connection
		m_text_main->SetForegroundColour(wxColor(0, 255, 0));
		m_text_main->SetLabel("... socket is now connected.");
		comm->m_ip->Hide();
		break;

		//Other events will trigger this action
	default:
		m_text_main->SetLabel("Unknown socket event!!!");
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

		m_text_main->SetLabel("connected");
		return true;
	}
	else
	{
		//or when we cannot connect with the provided adress, then display the error
		m_text_main->SetLabel("connection error");
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
