#ifndef PROJECTPIGEONFRAME_H
#define PROJECTPIGEONFRAME_H

#include <wx/arrstr.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/filename.h>
#include <wx/statbmp.h>
#include <wx/textctrl.h>
#include <wx/wx.h>

class ProjectPigeonFrame : public wxFrame {
public:
  explicit ProjectPigeonFrame(const wxString &title = "PigeonForge",
                              const wxPoint &pos = wxDefaultPosition,
                              const wxSize &size = wxSize(560, 340));

private:
  wxChoice *ProjectLanguageChoice{nullptr};
  wxChoice *projectTypeChoice{nullptr};
  wxTextCtrl *projectNameCtrl{nullptr};
  wxTextCtrl *projectPathCtrl{nullptr};

  void OnBrowse(wxCommandEvent &);
  void OnGenerate(wxCommandEvent &);
  void OnLanguageChange(wxCommandEvent &);

  wxDECLARE_EVENT_TABLE();
};

#endif
