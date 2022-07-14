#include "Panels.h"
#include <wx/wxprec.h>


class Communicate : public wxFrame
{
public:
    Communicate(const wxString& title);


    MainPanel* m_mp;
    RightPanel* m_rp;
    LeftPanel* m_lp;
    MidlePanel* m_midp;
    PgCounterPanel* m_pgp;

    wxPanel* m_parent;
    

  
   // wxString h = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
};