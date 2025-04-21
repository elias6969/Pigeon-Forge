#ifndef PATHMANAGER_H
#define PATHMANAGER_H

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class VirtualFileSystem {
public:
  explicit VirtualFileSystem(const std::string &basePath);
  std::string getFullPath(const std::string &relativePath) const;
  std::string readFile(const std::string &relativePath);

private:
  std::string baseDir;
};

class PathsManagerCtrl {
public:
  VirtualFileSystem vfs;
  std::string ImagesPath;
  std::string CPlusplusPath;
  std::string CSharpPath;
  std::string WebFramePath;

  // Constructor that sets everything up
  explicit PathsManagerCtrl(const std::string &basePath);
};

extern PathsManagerCtrl GlobalPaths;

#endif
