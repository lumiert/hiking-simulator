#pragma once

#include <string>
#include <memory>

class Sound {
private:
    int id;

public:
    Sound();
    virtual ~Sound() = default;

    virtual bool load(const std::string& path) = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setVolume(float volume) = 0;
};

using SoundPtr = std::shared_ptr<Sound>;
