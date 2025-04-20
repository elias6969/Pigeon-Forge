#include "ProjectPigeonFrame.h"
#include "filemanager.h"

#include <filesystem>
#include <map>
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

// ── Event table ────────────────────────────────────────────────
wxBEGIN_EVENT_TABLE(ProjectWizardFrame, wxFrame)
    EVT_BUTTON(wxID_HIGHEST + 1, ProjectWizardFrame::OnBrowse)
        EVT_BUTTON(wxID_HIGHEST + 2, ProjectWizardFrame::OnGenerate)
            wxEND_EVENT_TABLE()

    // ── Constructor ────────────────────────────────────────────────
    ProjectWizardFrame::ProjectWizardFrame(const wxString &title,
                                           const wxPoint &pos,
                                           const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
  /* dark pigeon palette */
  wxColour bg(30, 32, 34);       // near‑black
  wxColour fg(230, 230, 232);    // light grey
  wxColour accent(88, 140, 235); // pigeon‑blue
  SetBackgroundColour(bg);
  SetForegroundColour(fg);

  /* load icon + mascot bitmap (same dir as exe) */
  wxFileName exe(wxStandardPaths::Get().GetExecutablePath());
  wxString assetDir = exe.GetPath();
  SetIcon(wxIcon(assetDir + "/pigeon.ico", wxBITMAP_TYPE_ICO));
  wxBitmap logoBmp(assetDir + "/pigeon_64.png", wxBITMAP_TYPE_PNG);

  /* top mascot + title */
  wxStaticBitmap *logo = new wxStaticBitmap(this, wxID_ANY, logoBmp);
  wxStaticText *titleLbl = new wxStaticText(this, wxID_ANY, "PigeonForge");
  titleLbl->SetFont(
      wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
  titleLbl->SetForegroundColour(accent);

  /* form widgets */
  projectTypeChoice = new wxChoice(this, wxID_ANY);
  projectTypeChoice->Append("C++ Console");
  projectTypeChoice->Append("wxWidgets App");
  projectTypeChoice->Append("OpenGL");
  projectTypeChoice->SetSelection(0);

  projectNameCtrl = new wxTextCtrl(this, wxID_ANY, "MyProject");
  projectPathCtrl = new wxTextCtrl(this, wxID_ANY);
  wxButton *browseBtn = new wxButton(this, wxID_HIGHEST + 1, "Browse…");
  wxButton *generateBtn = new wxButton(this, wxID_HIGHEST + 2, "Generate");

  browseBtn->SetBackgroundColour(wxColour(50, 52, 54));
  generateBtn->SetBackgroundColour(accent);
  generateBtn->SetForegroundColour(wxColour(255, 255, 255));

  /* form grid */

  /* ── form grid (FIXED) ───────────────────────────────────────── */
  wxFlexGridSizer *form = new wxFlexGridSizer(2, 8, 8);
  auto addRow = [&](const wxString &label, wxWindow *field) {
    wxStaticText *lbl = new wxStaticText(this, wxID_ANY, label);
    lbl->SetForegroundColour(fg);
    form->Add(lbl, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    form->Add(field, 1, wxEXPAND);
  };

  addRow("Project type:", projectTypeChoice);
  addRow("Project name:", projectNameCtrl);

  /* Location row: label + custom h‑sizer */
  wxStaticText *locLbl = new wxStaticText(this, wxID_ANY, "Location:");
  locLbl->SetForegroundColour(fg);
  form->Add(locLbl, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);

  wxBoxSizer *pathRow = new wxBoxSizer(wxHORIZONTAL);
  pathRow->Add(projectPathCtrl, 1, wxEXPAND);
  pathRow->Add(browseBtn, 0, wxLEFT, 6);
  form->Add(pathRow, 1, wxEXPAND);

  form->AddGrowableCol(1, 1);

  /* master layout (unchanged) */
  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
  vbox->AddSpacer(10);
  vbox->Add(logo, 0, wxALIGN_CENTER | wxBOTTOM, 6);
  vbox->Add(titleLbl, 0, wxALIGN_CENTER | wxBOTTOM, 14);
  vbox->Add(form, 0, wxEXPAND | wxLEFT | wxRIGHT, 20);
  vbox->AddStretchSpacer(1);
  vbox->Add(generateBtn, 0, wxALIGN_CENTER | wxBOTTOM, 18);

  SetSizerAndFit(vbox);
}

// ── Browse handler ─────────────────────────────────────────────
void ProjectWizardFrame::OnBrowse(wxCommandEvent &) {
  wxDirDialog dlg(this, "Select Project Directory", wxEmptyString,
                  wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
  if (dlg.ShowModal() == wxID_OK)
    projectPathCtrl->SetValue(dlg.GetPath());
}

// ── Generate handler (logic unchanged) ─────────────────────────
void ProjectWizardFrame::OnGenerate(wxCommandEvent &) {
  wxString type = projectTypeChoice->GetStringSelection();
  wxString name = projectNameCtrl->GetValue();
  wxString path = projectPathCtrl->GetValue();

  if (type.IsEmpty() || name.IsEmpty() || path.IsEmpty()) {
    wxMessageBox("Please fill in all fields.", "Error", wxICON_ERROR);
    return;
  }

  namespace fs = std::filesystem;
  fs::path projDir = fs::path(path.ToStdString()) / name.ToStdString();
  for (auto dir : {"src", "include", "assets", "build", ".cache"})
    fs::create_directories(projDir / dir);

  /* locate templates */
  fs::path root =
      fs::path(getenv("HOME")) / "Documents" / "Pigeon-App" / "assets";
  fs::path set;
  bool fullTree = false;
  if (type == "wxWidgets App")
    set = root / "wxwidgets";
  else if (type == "OpenGL") {
    set = root / "OpenGL";
    fullTree = true;
  } else
    set = root / "c++_console";

  std::map<std::string, std::string> vars = {
      {"PROJECT_NAME", name.ToStdString()}};

  try {
    if (fullTree)
      FileManager::CopyDirectory(set, projDir, vars);
    else {
      FileManager::CopyTemplate(set / "main.cpp.template",
                                projDir / "src" / "main.cpp", vars);
      FileManager::CopyTemplate(set / "CMakeLists.txt.template",
                                projDir / "CMakeLists.txt", vars);
    }
  } catch (const std::exception &ex) {
    wxMessageBox("Template error:\n" + wxString(ex.what()), "Error",
                 wxICON_ERROR);
    return;
  }

  wxMessageBox("Project created at:\n" + projDir.string(), "Success",
               wxICON_INFORMATION);
}
