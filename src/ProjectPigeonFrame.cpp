#include "ProjectPigeonFrame.h"
#include "PathManager.h"
#include "filemanager.h"

#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include <wx/arrstr.h>
#include <wx/choice.h>
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/wx.h>

wxBEGIN_EVENT_TABLE(ProjectPigeonFrame, wxFrame)
    EVT_BUTTON(wxID_HIGHEST + 1, ProjectPigeonFrame::OnBrowse)
        EVT_BUTTON(wxID_HIGHEST + 2, ProjectPigeonFrame::OnGenerate)
            wxEND_EVENT_TABLE()

                ProjectPigeonFrame::ProjectPigeonFrame(const wxString &title,
                                                       const wxPoint &pos,
                                                       const wxSize &size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size) {
  // Dark pigeon palette
  wxColour bg(30, 32, 34);
  wxColour fg(230, 230, 232);
  wxColour accent(88, 140, 235);
  SetBackgroundColour(bg);
  SetForegroundColour(fg);

  // Load icon + logo
  SetIcon(wxIcon((GlobalPaths.ImagesPath / "pigeon.ico").string(), wxBITMAP_TYPE_ICO));
  wxBitmap logoBmp((GlobalPaths.ImagesPath / "pigeon_64.png").string(), wxBITMAP_TYPE_PNG);
  wxStaticBitmap *logo = new wxStaticBitmap(this, wxID_ANY, logoBmp);

  // Title label
  wxStaticText *titleLbl = new wxStaticText(this, wxID_ANY, "PigeonForge");
  titleLbl->SetFont(
      wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
  titleLbl->SetForegroundColour(accent);

  // Language choice
  ProjectLanguageChoice = new wxChoice(this, wxID_ANY);
  ProjectLanguageChoice->Append("C++");
  ProjectLanguageChoice->Append("C#");
  ProjectLanguageChoice->Append("Web");
  ProjectLanguageChoice->SetSelection(0);
  ProjectLanguageChoice->Bind(wxEVT_CHOICE,
                              &ProjectPigeonFrame::OnLanguageChange, this);

  // Template choice
  projectTypeChoice = new wxChoice(this, wxID_ANY);
  OnLanguageChange(*(new wxCommandEvent())); // initialize

  // Name and path controls
  projectNameCtrl = new wxTextCtrl(this, wxID_ANY, "MyProject");
  projectPathCtrl = new wxTextCtrl(this, wxID_ANY);
  wxButton *browseBtn = new wxButton(this, wxID_HIGHEST + 1, "Browse…");
  wxButton *generateBtn = new wxButton(this, wxID_HIGHEST + 2, "Generate");
  browseBtn->SetBackgroundColour(wxColour(50, 52, 54));
  generateBtn->SetBackgroundColour(accent);
  generateBtn->SetForegroundColour(wxColour(255, 255, 255));

  // Layout: form
  wxFlexGridSizer *form = new wxFlexGridSizer(2, 8, 8);
  auto addRow = [&](const wxString &label, wxWindow *field) {
    wxStaticText *lbl = new wxStaticText(this, wxID_ANY, label);
    lbl->SetForegroundColour(fg);
    form->Add(lbl, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    form->Add(field, 1, wxEXPAND);
  };
  addRow("Language:", ProjectLanguageChoice);
  addRow("Template:", projectTypeChoice);
  addRow("Project name:", projectNameCtrl);
  wxStaticText *locLbl = new wxStaticText(this, wxID_ANY, "Location:");
  locLbl->SetForegroundColour(fg);
  form->Add(locLbl, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
  wxBoxSizer *pathRow = new wxBoxSizer(wxHORIZONTAL);
  pathRow->Add(projectPathCtrl, 1, wxEXPAND);
  pathRow->Add(browseBtn, 0, wxLEFT, 6);
  form->Add(pathRow, 1, wxEXPAND);
  form->AddGrowableCol(1, 1);

  wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
  vbox->AddSpacer(10);
  vbox->Add(logo, 0, wxALIGN_CENTER | wxBOTTOM, 6);
  vbox->Add(titleLbl, 0, wxALIGN_CENTER | wxBOTTOM, 14);
  vbox->Add(form, 0, wxEXPAND | wxLEFT | wxRIGHT, 20);
  vbox->AddStretchSpacer(1);
  vbox->Add(generateBtn, 0, wxALIGN_CENTER | wxBOTTOM, 18);

  SetSizerAndFit(vbox);
}

void ProjectPigeonFrame::OnLanguageChange(wxCommandEvent &) {
  wxString lang = ProjectLanguageChoice->GetStringSelection();
  projectTypeChoice->Clear();

  if (lang == "C++") {
    projectTypeChoice->Append("C++ / Console");
    projectTypeChoice->Append("C++ / wxWidgets");
    projectTypeChoice->Append("C++ / OpenGL");
    projectTypeChoice->Append("C++ / Raylib");
  } else if (lang == "C#") {
    projectTypeChoice->Append("C# / .NET Console");
  } else if (lang == "Web") {
    projectTypeChoice->Append("Web / React App");
  }
  projectTypeChoice->SetSelection(0);
}

void ProjectPigeonFrame::OnBrowse(wxCommandEvent &) {
  wxDirDialog dlg(this, "Select Project Directory", wxEmptyString,
                  wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
  if (dlg.ShowModal() == wxID_OK)
    projectPathCtrl->SetValue(dlg.GetPath());
}

void ProjectPigeonFrame::OnGenerate(wxCommandEvent &) {
  wxString Language = ProjectLanguageChoice->GetStringSelection();
  wxString Type = projectTypeChoice->GetStringSelection();
  wxString name = projectNameCtrl->GetValue();
  wxString path = projectPathCtrl->GetValue();

  if (Language.IsEmpty() || Type.IsEmpty() || name.IsEmpty() ||
      path.IsEmpty()) {
    wxMessageBox("Please fill in all fields.", "Error", wxICON_ERROR);
    return;
  }

  namespace fs = std::filesystem;
  fs::path projDir = fs::path(path.ToStdString()) / name.ToStdString();

  // Choose folders based on language
  std::vector<std::string> folders;
  if (Language == "C++") {
    folders = {"src", "include", "assets", "build", ".cache"};
  } else if (Language == "Web") {
    folders = {"src", "public"};
  } else if (Language == "C#") {
    folders = {""};
  }

  // Create project directories
  for (const auto &dir : folders) {
    if (!dir.empty())
      fs::create_directories(projDir / dir);
  }

  fs::path root;
  fs::path set;
  bool fullTree = false;

  if (Language == "C++") {
    if (Type == "C++ / Console") {
      set = GlobalPaths.CPlusplusPath / "c++_console";
    } else if (Type == "C++ / wxWidgets") {
      set = GlobalPaths.CPlusplusPath / "wxwidgets";
    } else if (Type == "C++ / OpenGL") {
      set = GlobalPaths.CPlusplusPath / "OpenGL";
      fullTree = true;
    } else if (Type == "C++ / Raylib") {
      set = GlobalPaths.CPlusplusPath / "Raylib";
      fullTree = true;
    }
  } else if (Language == "C#") {
    set = GlobalPaths.CSharpPath;
    fullTree = true;
  } else if (Language == "Web") {
    set = GlobalPaths.WebFramePath;
    fullTree = true;
  } else {
    wxMessageBox("Unknown language selected!", "Error", wxICON_ERROR);
    return;
  }

  std::map<std::string, std::string> vars = {
      {"PROJECT_NAME", name.ToStdString()}};

  try {
    // Copy templates
    if (fullTree) {
      FileManager::CopyDirectory(set, projDir, vars);
    } else {
      if (Language == "C++") {
        FileManager::CopyTemplate(set / "main.cpp.template",
                                  projDir / "src" / "main.cpp", vars);
        FileManager::CopyTemplate(set / "CMakeLists.txt.template",
                                  projDir / "CMakeLists.txt", vars);
      } else if (Language == "Web") {
        FileManager::CopyTemplate(set / "index.html",
                                  projDir / "public" / "index.html", vars);
      } else if (Language == "C#") {
      }
    }

    // Change working directory for terminal commands
    fs::current_path(projDir);

    if (Language == "Web") {
      std::system("npm init -y");
      std::system("npm install");
    }
    if (Language == "C#") {
      wxString projectName = projectNameCtrl->GetValue();
      std::string escapedName = "\"" + std::string(projectName.mb_str()) + "\"";
      std::string command = "dotnet new console -n " + escapedName;
      std::system(command.c_str());
    }

  } catch (const std::exception &ex) {
    wxMessageBox("Template error:\n" + wxString(ex.what()), "Error",
                 wxICON_ERROR);
    return;
  }

  wxMessageBox("Project created at:\n" + projDir.string(), "Success",
               wxICON_INFORMATION);
}
