#include "Main.h"
#include "Communicate.h"

#include "wx/config.h"

#ifndef wxHAS_IMAGES_IN_RESOURCES
#include "../sample.xpm"
#endif

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{

    // we're using wxConfig's "create-on-demand" feature: it will create the
        // config object when it's used for the first time. It has a number of
        // advantages compared with explicitly creating our wxConfig:
        //  1) we don't pay for it if we don't use it
        //  2) there is no danger to create it twice

        // application and vendor name are used by wxConfig to construct the name
        // of the config file/registry key and must be set before the first call
        // to Get() if you want to override the default values (the application
        // name is the name of the executable and the vendor name is the same)
        SetVendorName("Skalarki");
    SetAppName("DCDU_config"); // not needed, it's the default value

    wxConfigBase* pConfig = wxConfigBase::Get();

    // uncomment this to force writing back of the defaults for all values
    // if they're not present in the config - this can give the user an idea
    // of all possible settings for this program
    pConfig->SetRecordDefaults();


    //wxMessageBox(wxString::Format(wxT("%i"), w)); <---used for debugging 
    //initialization of main communicate frame
    Communicate* communicate = new Communicate(wxT("Widgets communicate"));
    //displaing the app window in full screen mode
    communicate->Show(true);
    communicate->ShowFullScreen(true);
    return true;
}

int MyApp::OnExit()
{
    // clean up: Set() returns the active config object as Get() does, but unlike
    // Get() it doesn't try to create one if there is none (definitely not what
    // we want here!)
    delete wxConfigBase::Set((wxConfigBase*)NULL);

    return 0;
}