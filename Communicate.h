#include "Panels.h"
#include <wx/wxprec.h>

//this class will handle the communication between the panels
class Communicate : public wxFrame
{
public:
    Communicate(const wxString& title);

    //declare all panels and parent panel
    MainPanel* m_mp;
    RightPanel* m_rp;
    LeftPanel* m_lp;
    MidlePanel* m_midp;
    PgCounterPanel* m_pgp;
    wxPanel* m_parent;

};