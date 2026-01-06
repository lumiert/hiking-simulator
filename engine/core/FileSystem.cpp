#include "FileSystem.h"
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

bool FileSystem::fileExists(const std::string& path) {
    return fs::exists(path) && fs::is_regular_file(path);
}

bool FileSystem::dirExists(const std::string& path) {
    return fs::exists(path) && fs::is_directory(path);
}

std::string FileSystem::readTextFile(const std::string& path) {
    if (!fileExists(path)) return "";

    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FileSystem::writeTextFile(const std::string& path, const std::string& content) {
    std::ofstream file(path);
    if (!file.is_open()) return false;
    file << content;
    file.close();
    return true;
}

std::vector<std::string> FileSystem::listDirectory(const std::string& path) {
    std::vector<std::string> files;
    if (!dirExists(path)) return files;

    for (const auto& entry : fs::directory_iterator(path)) {
        files.push_back(entry.path().string());
    }
    return files;
}

std::string FileSystem::getDirectory(const std::string& path) {
    return fs::path(path).parent_path().string();
}

std::string FileSystem::getFilename(const std::string& path) {
    return fs::path(path).filename().string();
}

std::string FileSystem::getExtension(const std::string& path) {
    return fs::path(path).extension().string();
}

std::string FileSystem::normalizePath(const std::string& path) {
    return fs::path(path).lexically_normal().string();
}
