#include <wx/wx.h>
#include "ProjectPigeonFrame.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    ProjectWizardFrame* frame = new ProjectWizardFrame("Project Maker");
    frame->Show(true);
    return true;
}
