#pragma once

#include "Sound.h"
#include "Music.h"
#include <memory>
#include <unordered_map>

class AudioSystem {
private:
    static AudioSystem* instance;
    std::unordered_map<std::string, SoundPtr> sounds;
    std::unordered_map<std::string, MusicPtr> music;
    float masterVolume;

    AudioSystem();

public:
    ~AudioSystem();
    static AudioSystem& getInstance();

    void loadSound(const std::string& name, const std::string& path);
    void loadMusic(const std::string& name, const std::string& path);

    void playSound(const std::string& name);
    void playMusic(const std::string& name);

    void setMasterVolume(float volume) { masterVolume = volume; }
    float getMasterVolume() const { return masterVolume; }
};
