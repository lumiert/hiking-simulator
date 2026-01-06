#pragma once

#include <string>
#include <vector>

class FileSystem {
public:
    static bool fileExists(const std::string& path);
    static bool dirExists(const std::string& path);
    static std::string readTextFile(const std::string& path);
    static bool writeTextFile(const std::string& path, const std::string& content);
    static std::vector<std::string> listDirectory(const std::string& path);
    static std::string getDirectory(const std::string& path);
    static std::string getFilename(const std::string& path);
    static std::string getExtension(const std::string& path);
    static std::string normalizePath(const std::string& path);
};
