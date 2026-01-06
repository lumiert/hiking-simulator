#pragma once

#include <string>
#include <unordered_map>

class Config {
private:
    static Config* instance;
    std::unordered_map<std::string, std::string> values;

    Config();

public:
    static Config& getInstance();

    void load(const std::string& filename);
    void save(const std::string& filename);

    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key, const std::string& defaultValue = "");

    bool getBool(const std::string& key, bool defaultValue = false);
    int getInt(const std::string& key, int defaultValue = 0);
    float getFloat(const std::string& key, float defaultValue = 0.0f);
};
