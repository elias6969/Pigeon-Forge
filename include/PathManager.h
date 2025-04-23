#pragma once
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

class VirtualFileSystem {
public:
    VirtualFileSystem(const std::string& basePath);
    fs::path       getFullPath(const fs::path& relativePath) const;
    std::string    readFile    (const fs::path& relativePath);

private:
    fs::path baseDir;
};

class PathsManagerCtrl {
public:
    PathsManagerCtrl();

    fs::path ImagesPath;
    fs::path CPlusplusPath;
    fs::path CSharpPath;
    fs::path WebFramePath;

private:
    VirtualFileSystem vfs;
};

// global instance
extern PathsManagerCtrl GlobalPaths;
