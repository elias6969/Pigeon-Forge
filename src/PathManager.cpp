#include "PathManager.h"

#include <fstream>
#include <sstream>
#include <wx/stdpaths.h>
#include <wx/filename.h>

namespace fs = std::filesystem;

VirtualFileSystem::VirtualFileSystem(const std::string& basePath) {
    // 1. Find where the EXE lives
    wxFileName exe(wxStandardPaths::Get().GetExecutablePath());
    fs::path exeDir(std::string(exe.GetPath().mb_str()));

    // 2. Resolve basePath (e.g. "../assets") against exeDir
    fs::path candidate = fs::path(basePath);
    if (candidate.is_relative())
        candidate = exeDir / candidate;

    // 3. Canonicalize (fold "..", symlinks, etc.)
    fs::path canon = fs::weakly_canonical(candidate);

    // 4. Validate it exists and is a directory
    if (!fs::exists(canon) || !fs::is_directory(canon)) {
        throw std::runtime_error("Assets folder not found: " + canon.string());
    }

    baseDir = canon;
}

fs::path VirtualFileSystem::getFullPath(const fs::path& relativePath) const {
    return baseDir / relativePath;
}

std::string VirtualFileSystem::readFile(const fs::path& relativePath) {
    fs::path full = getFullPath(relativePath);
    if (!fs::exists(full))
        throw std::runtime_error("File not found: " + full.string());

    std::ifstream file(full, std::ios::binary);
    if (!file)
        throw std::runtime_error("Cannot open file: " + full.string());

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

PathsManagerCtrl::PathsManagerCtrl()
  // Adjust this string if your exe lives elsewhere; 
  // "../assets" assumes your build/bin is one level under your project root.
  : vfs("../assets")
{
    ImagesPath     = vfs.getFullPath("Images");
    CPlusplusPath  = vfs.getFullPath("C++_FrameWorks");
    CSharpPath     = vfs.getFullPath("C#_FrameWorks");
    WebFramePath   = vfs.getFullPath("Web_Frameworks");  // <-- fixed spelling to match your folder

    // Optional: immediate sanity check
    for (auto p : { ImagesPath, CPlusplusPath, CSharpPath, WebFramePath }) {
        if (!fs::exists(p))
            throw std::runtime_error("Missing subfolder: " + p.string());
    }
}

// define the global instance
PathsManagerCtrl GlobalPaths;
