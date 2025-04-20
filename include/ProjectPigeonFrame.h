#ifndef PROJECTPIGEONFRAME_H
#define PROJECTPIGEONFRAME_H

#include <wx/wx.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/statbmp.h>
#include <wx/filename.h>

class ProjectWizardFrame : public wxFrame
{
public:
    explicit ProjectWizardFrame(const wxString& title = "PigeonForge",
                                const wxPoint& pos   = wxDefaultPosition,
                                const wxSize&  size  = wxSize(560, 340));

private:
    /* widgets */
    wxChoice*   projectTypeChoice {nullptr};
    wxTextCtrl* projectNameCtrl   {nullptr};
    wxTextCtrl* projectPathCtrl   {nullptr};

    /* events */
    void OnBrowse  (wxCommandEvent&);
    void OnGenerate(wxCommandEvent&);

    wxDECLARE_EVENT_TABLE();
};

#endif
