#include "Main.h"
#include "Communicate.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{

    //wxMessageBox(wxString::Format(wxT("%i"), w)); <---used for debugging 
    //initialization of main communicate frame
    Communicate* communicate = new Communicate(wxT("Widgets communicate"));
    //displaing the app window in full screen mode
    communicate->Show(true);
    communicate->ShowFullScreen(true);
    return true;
}