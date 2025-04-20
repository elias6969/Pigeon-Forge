#pragma once
#include <string>
#include <map>
#include <set>
#include <filesystem>
#include <fstream>
#include <sstream>

class FileManager
{
public:
    /* ───────── basic helpers (unchanged) ───────── */
    static std::string LoadAndReplaceTemplate(const std::filesystem::path& tpl,
                                              const std::map<std::string,std::string>& reps)
    {
        std::ifstream in(tpl, std::ios::binary);
        if(!in.is_open())
            throw std::runtime_error("Cannot open template: " + tpl.string());

        std::ostringstream ss; ss << in.rdbuf(); in.close();
        std::string data = ss.str();

        for(const auto& [k,v] : reps)
        {
            std::string ph = "{{" + k + "}}";
            size_t pos = 0;
            while((pos = data.find(ph,pos)) != std::string::npos)
            {
                data.replace(pos, ph.size(), v);
                pos += v.size();
            }
        }
        return data;
    }

    static void WriteFile(const std::filesystem::path& dst, const std::string& data)
    {
        std::filesystem::create_directories(dst.parent_path());
        std::ofstream out(dst, std::ios::binary);
        if(!out.is_open())
            throw std::runtime_error("Cannot write file: " + dst.string());
        out << data;
    }

    static void CopyTemplate(const std::filesystem::path& tpl,
                             const std::filesystem::path& dst,
                             const std::map<std::string,std::string>& reps)
    {
        WriteFile(dst, LoadAndReplaceTemplate(tpl, reps));
    }

    /* ───────── NEW: smart recursive copy ───────── */
    static void CopyDirectory(const std::filesystem::path& srcDir,
                              const std::filesystem::path& dstDir,
                              const std::map<std::string,std::string>& reps)
    {
        using namespace std::filesystem;
        copy(srcDir, dstDir,
             copy_options::recursive |
             copy_options::copy_symlinks |
             copy_options::update_existing);

        auto isText = [](const path& p)
        {
            static const std::set<std::string> ext{
                ".cpp",".cc",".c",".h",".hpp",".inl",".cmake",
                ".txt",".md",".glsl",".vert",".frag",".template"};
            return ext.count(p.extension().string()) > 0;
        };

        for(auto& e : recursive_directory_iterator(dstDir))
        {
            if(!e.is_regular_file()) continue;

            /* .template  →  strip suffix & substitute */
            if(e.path().extension() == ".template")
            {
                path newPath = e.path();
                newPath.replace_extension("");          // drop '.template'
                WriteFile(newPath,
                          LoadAndReplaceTemplate(e.path(), reps));
                remove(e.path());                       // remove the *.template file
            }
            /* normal text file  →  substitute placeholders in‑place */
            else if(isText(e.path()))
            {
                WriteFile(e.path(),
                          LoadAndReplaceTemplate(e.path(), reps));
            }
            /* binary / JSON / symlinks are left untouched */
        }
    }
};
