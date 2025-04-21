#include <wx/wx.h>
#include "ProjectPigeonFrame.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    // Create and show the window
    ProjectPigeonFrame* frame = new ProjectPigeonFrame("Pigeon Forge");
    frame->Show(true);
    return true; 
}
