#include <wx/wx.h>
#include <wx/panel.h>

class MainPanel : public wxPanel
{
public:

   
    MainPanel(wxPanel* parent);

    void OnMsgPlus(wxCommandEvent& event);
    void OnMsgMinus(wxCommandEvent& event);
    void OnPOEPlus(wxCommandEvent& event);
    void OnPOEMinus(wxCommandEvent& event);
  
    //buttons for simulation
    wxButton* msg_plus;
    wxButton* msg_minus;
    wxButton* m_POE_plus;
    wxButton* m_POE_minus;
    wxStaticText* m_text_main;
    wxPanel* m_parent;
    wxString text_main;

    wxFont* main_font;

    int count_msg;
    int count_poe;
};

class RightPanel : public wxPanel
{
public:
    RightPanel(wxPanel* parent);
    void OnRightUp(wxCommandEvent& event);
    void OnRightDown(wxCommandEvent& event);
    void OnSetText(wxCommandEvent& event);
    wxButton* m_right_up;
    wxButton* m_right_down;
    wxStaticText* m_text;
    wxString text_r;
    int count_r;
    wxStaticText* m_text_r;

    wxFont* right_font;

};

class LeftPanel : public wxPanel
{
public: 
    LeftPanel(wxPanel* parent);
    wxButton* m_left_up;
    wxButton* m_left_down;
    void OnLeftUp(wxCommandEvent& event);
    void OnLeftDown(wxCommandEvent& event);

    void OnSetText(wxCommandEvent& event);
    wxStaticText* m_text_l;
    //wxStaticText* m_text;
    wxString text_l;
    int count_l;
    wxFont* left_font;

};

class MidlePanel : public wxPanel
{
public:
    MidlePanel(wxPanel* parent);
    wxStaticText* m_text_mid;
    void OnSetText(wxCommandEvent& event);
    wxBoxSizer* sizer_1;
    //wxStaticText* m_text;
    wxString text_mid;
    int count_m;
    wxFont* mid_font;

};

class PgCounterPanel : public wxPanel
{
public:
    PgCounterPanel(wxPanel* parent);
    wxStaticText* m_text_pg;
    void OnSetText(wxCommandEvent& event);
    wxBoxSizer* sizer_1;
    //wxStaticText* m_text;
    //wxString text_mid;
    int count_pg;
    wxFont* pg_font;

};
const int ID_MSG_PLUS = 101;
const int ID_MSG_MINUS = 102;
const int ID_POE_PLUS = 103;
const int ID_POE_MINUS = 104;
const int ID_LU = 105;
const int ID_LD = 106;
const int ID_RU = 107;
const int ID_RD = 108;

