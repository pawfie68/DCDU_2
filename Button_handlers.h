#pragma once

//======================================================================================================================================
//Events on buttons in main panel//
//======================================================================================================================================

void MainPanel::OnPrintEvent(wxCommandEvent& WXUNUSED(event))
{
	/* communicate class to handle data transfer between the panels */
	Communicate* comm = (Communicate*)m_parent->GetParent();
	comm->m_mp->m_text_main->SetLabel("");
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
	comm->m_rp->m_text_r->SetLabel(text_r);
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
	comm->m_rp->m_text_r->SetLabel(text_r);
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