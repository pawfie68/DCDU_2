#include <wx/wx.h>
#include <wx/panel.h>

#include "wx/richtext/richtextctrl.h"
#include "wx/richtext/richtextstyles.h"
#include "wx/richtext/richtextxml.h"
#include "wx/richtext/richtexthtml.h"
#include "wx/richtext/richtextformatdlg.h"
#include "wx/richtext/richtextsymboldlg.h"
#include "wx/richtext/richtextstyledlg.h"
#include "wx/richtext/richtextprint.h"
#include "wx/richtext/richtextimagedlg.h"


#include <wx/socket.h>


//Panels, buttons, text boxes and events are initialized here



class MainPanel : public wxPanel
{
public:

    //MainPanel -> to top box of the screen
    MainPanel(wxPanel* parent);
    ~MainPanel();
    //parent panel of the app
    wxPanel* m_parent;

    //events for MSG and PGE button handling    
    void OnMsgPlus(wxCommandEvent& event);
    void OnMsgMinus(wxCommandEvent& event);
    void OnPOEPlus(wxCommandEvent& event);
    void OnPOEMinus(wxCommandEvent& event);

    void OnPrintEvent(wxCommandEvent& event);
    void ButtonClicked();

    //     void OnButtonCombination(wxCommandEvent& event);
    //buttons for simulating the operation of the device
    wxButton* msg_plus;
    wxButton* msg_minus;
    wxButton* m_POE_plus;
    wxButton* m_POE_minus;
    wxButton* m_print;

    wxString Received;

    //wxRichText
    wxRichTextCtrl* m_rtext_main;
    //text box for main window
  //  wxStaticText* m_text_main;

    //auxiliary text variable
    wxString text_main;

    //font for handling custon fonts for main screen 
    wxFont* main_font;
    //socket for TCP/IP connection
    wxSocketClient* m_sock;
    //flag to determine if socket is buissy processing data/transfers
    bool m_busy;

    //adress to handle TCP/IP
    wxSockAddress* m_adress;


    wxIPaddress* addr;
    wxIPV4address addr4;

    // event handlers for Socket menu
    // when connection is established
    void OnOpenConnection(wxCommandEvent& event);
    //when data arrived
    void OnSocketEvent(wxSocketEvent& event);

    //open connection with specified parameters
    bool OpenConnection(wxSockAddress::Family family);
    void Disconnect();


    const unsigned char len = 32;


  //  wxIPaddress* addr;
   // wxIPV4address addr4;


};

///////////////////////////////////////////////// IpPanel //////////////////////////////////////////////////
/// ____________________________________________________________________________________________________ ///
/// This panel will show up when no config file is present in the system, or in case when data stored in ///
/// the config file are corrupted ---------------------------------------------------------------------- ///
////////////////////////////////////////////////////////////////////////////////////////////////////////////

class IpPanel : public wxPanel
{
public:

    //MainPanel -> to top box of the screen
    IpPanel(wxPanel* parent);
    ~IpPanel();

    void NetConfig();

    char count_msg = 1;


    char ip_to_display[50];
    char port_to_dispaly[8];


    //char count_poe = -1;
    char count_poe[22];

    //flag for switching key functions between config and normal 
    bool Config_flag = true;

    wxStaticText* m_text_ip;

    wxString ip_from_cfg;
    wxString port_from_cfg;

    //sth to deal with cfg file
    wxString m_IP;
    wxString m_PORT;

    wxTextCtrl* m_ip_1;
    wxTextCtrl* m_ip_2;
    wxTextCtrl* m_ip_3;
    wxTextCtrl* m_ip_4;
    wxTextCtrl* m_ip_5;
    wxTextCtrl* m_ip_6;
    wxTextCtrl* m_ip_7;
    wxTextCtrl* m_ip_8;
    wxTextCtrl* m_ip_9;
    wxTextCtrl* m_ip_10;
    wxTextCtrl* m_ip_11;
    wxTextCtrl* m_ip_12;
    wxTextCtrl* m_ip_13;
    wxTextCtrl* m_ip_14;
    wxTextCtrl* m_ip_15;
    wxTextCtrl* m_ip_16;
    wxTextCtrl* m_ip_17;
    wxTextCtrl* m_ip_18;

    wxStaticText* dot1;
    wxStaticText* dot2;
    wxStaticText* dot3;
    wxStaticText* semicolon;

    wxStaticText* ip_setup_desc;

    wxTextCtrl* m_ip_ctrl;
    wxTextCtrl* m_port_ctrl;
    wxString hostname;
    wxString port;


    wxFont* ip_font;
};


class RightPanel : public wxPanel
{
public:
    //RightPanel -> to right bottom box of the screen
    RightPanel(wxPanel* parent);
    //buttons events for simulating the operation of the device
    void OnRightUp(wxCommandEvent& event);
    void OnRightDown(wxCommandEvent& event);
    void OnSetText(wxCommandEvent& event);

    //buttons for simulating the operation of the device
    wxButton* m_right_up;
    wxButton* m_right_down;

    //text box for main window
    wxStaticText* m_text;
    wxString text_r;
    wxStaticText* m_text_r;
    //some counters dot
    int count_r;

    // font for handling custon fonts for right screen
    wxFont* right_font;

    int m_IP_value;


};

class LeftPanel : public wxPanel
{
public:
    //LeftPanel -> to Left bottom box of the screen
    LeftPanel(wxPanel* parent);

    //buttons for simulating the operation of the device
    wxButton* m_left_up;
    wxButton* m_left_down;

    //buttons events for simulating the operation of the device
    void OnLeftUp(wxCommandEvent& event);
    void OnLeftDown(wxCommandEvent& event);
    void OnSetText(wxCommandEvent& event);

    wxStaticText* m_text_l;

    //some auxiliary variables
    wxString text_l;
    int count_l;

    // font for handling custon fonts
    wxFont* left_font;

    int m_IP_index;

};

class MidlePanel : public wxPanel
{
public:

    //MidlePanel -> to Midle bottom box of the screen
    MidlePanel(wxPanel* parent);
    //text box
    wxStaticText* m_text_mid;
    void OnSetText(wxCommandEvent& event);
    //auxiliary variables
    wxString text_mid;
    int count_m;

    // font for handling custon fonts
    wxFont* mid_font;

};

class PgCounterPanel : public wxPanel
{
public:

    //PgCounterPanel -> additional panel placed at right bottom of 
    //midle panel for displaing of page numbers

    PgCounterPanel(wxPanel* parent);
    //text box
    wxStaticText* m_text_pg;
    void OnSetText(wxCommandEvent& event);

    wxBoxSizer* sizer_1;
    //wxStaticText* m_text;
    //wxString text_mid;
    int count_pg;

    // font for handling custon fonts
    wxFont* pg_font;

};

//indyvidual IDs of buttons for handling the interactions
const int ID_MSG_PLUS = 101;
const int ID_MSG_MINUS = 102;
const int ID_POE_PLUS = 103;
const int ID_POE_MINUS = 104;
const int ID_PRINT = 105;
const int ID_LU = 106;
const int ID_LD = 107;
const int ID_RU = 108;
const int ID_RD = 109;
const int ID_COMBINED = 110;

