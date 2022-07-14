#include <wx/wx.h>
#include <wx/panel.h>

//Panels, buttons, text boxes and events are initialized here

class MainPanel : public wxPanel
{
public:

    //MainPanel -> to top box of the screen
    MainPanel(wxPanel* parent);

    //parent panel of the app
    wxPanel* m_parent;

    //events for MSG and PGE button handling    
    void OnMsgPlus(wxCommandEvent& event);
    void OnMsgMinus(wxCommandEvent& event);
    void OnPOEPlus(wxCommandEvent& event);
    void OnPOEMinus(wxCommandEvent& event);
  
    //buttons for simulating the operation of the device
    wxButton* msg_plus;
    wxButton* msg_minus;
    wxButton* m_POE_plus;
    wxButton* m_POE_minus;

    //text box for main window
    wxStaticText* m_text_main;

    //auxiliary text variable
    wxString text_main;

    //font for handling custon fonts for main screen 
    wxFont* main_font;

    //some counters dot
    int count_msg;
    int count_poe;
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
const int ID_LU = 105;
const int ID_LD = 106;
const int ID_RU = 107;
const int ID_RD = 108;

