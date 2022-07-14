#include "main.h"
#include "Communicate.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    int w = int(wxSystemSettings::GetMetric(wxSYS_SCREEN_X));
    //wxMessageBox(wxString::Format(wxT("%i"), w));
    Communicate* communicate = new Communicate(wxT("Widgets communicate"));
    communicate->Show(true);
    communicate->ShowFullScreen(true);
    return true;
}