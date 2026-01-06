#pragma once

#include <string>

class SDLAudio {
private:
    static SDLAudio* instance;

    SDLAudio();

public:
    ~SDLAudio();
    static SDLAudio& getInstance();

    bool initialize();
    void shutdown();
};
