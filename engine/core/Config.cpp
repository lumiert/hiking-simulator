#include "Config.h"
#include "FileSystem.h"
#include <sstream>

Config* Config::instance = nullptr;

Config::Config() {}

Config& Config::getInstance() {
    if (!instance) {
        instance = new Config();
    }
    return *instance;
}

void Config::load(const std::string& filename) {
    std::string content = FileSystem::readTextFile(filename);
    std::istringstream stream(content);
    std::string line;

    while (std::getline(stream, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            set(key, value);
        }
    }
}

void Config::save(const std::string& filename) {
    std::string content;
    for (const auto& pair : values) {
        content += pair.first + "=" + pair.second + "\n";
    }
    FileSystem::writeTextFile(filename, content);
}

void Config::set(const std::string& key, const std::string& value) {
    values[key] = value;
}

std::string Config::get(const std::string& key, const std::string& defaultValue) {
    auto it = values.find(key);
    return it != values.end() ? it->second : defaultValue;
}

bool Config::getBool(const std::string& key, bool defaultValue) {
    std::string value = get(key);
    if (value.empty()) return defaultValue;
    return value == "true" || value == "1" || value == "yes";
}

int Config::getInt(const std::string& key, int defaultValue) {
    std::string value = get(key);
    if (value.empty()) return defaultValue;
    return std::stoi(value);
}

float Config::getFloat(const std::string& key, float defaultValue) {
    std::string value = get(key);
    if (value.empty()) return defaultValue;
    return std::stof(value);
}
