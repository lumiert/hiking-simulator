#pragma once

#include "Sound.h"
#include <string>
#include <memory>

class Music {
private:
    int id;

public:
    Music();
    virtual ~Music() = default;

    virtual bool load(const std::string& path) = 0;
    virtual void play(int loops = -1) = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void setVolume(float volume) = 0;
};

using MusicPtr = std::shared_ptr<Music>;
