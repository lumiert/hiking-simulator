#include "../audio/AudioSystem.h"

AudioSystem* AudioSystem::instance = nullptr;

AudioSystem::AudioSystem() : masterVolume(1.0f) {}

AudioSystem::~AudioSystem() {}

AudioSystem& AudioSystem::getInstance() {
    if (!instance) {
        instance = new AudioSystem();
    }
    return *instance;
}

void AudioSystem::loadSound(const std::string& name, const std::string& path) {}

void AudioSystem::loadMusic(const std::string& name, const std::string& path) {}

void AudioSystem::playSound(const std::string& name) {}

void AudioSystem::playMusic(const std::string& name) {}
