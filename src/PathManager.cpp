#include "PathManager.h"
#include <fstream>
#include <sstream>

// --- VirtualFileSystem Implementation ---

VirtualFileSystem::VirtualFileSystem(const std::string& basePath)
    : baseDir(basePath) {}

std::string VirtualFileSystem::getFullPath(const std::string& relativePath) const {
    return (fs::path(baseDir) / relativePath).string();
}

std::string VirtualFileSystem::readFile(const std::string& relativePath) {
    std::ifstream file(getFullPath(relativePath));
    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

PathsManagerCtrl::PathsManagerCtrl(const std::string& basePath)
    : vfs(basePath)
{
    ImagesPath     = vfs.getFullPath("Images/");
    CPlusplusPath  = vfs.getFullPath("C++_FrameWorks/");
    CSharpPath     = vfs.getFullPath("C#_FrameWorks/");
    WebFramePath   = vfs.getFullPath("Web_FrameWorks/");
}

// --- Define the global instance ---
PathsManagerCtrl GlobalPaths("../assets/");
