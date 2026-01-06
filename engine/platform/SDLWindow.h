#pragma once

#include <SDL2/SDL.h>
#include "../math/MathTypes.h"

class SDLWindow {
private:
    SDL_Window* window;
    int width, height;
    bool vsync;

public:
    SDLWindow();
    ~SDLWindow();

    bool create(const std::string& title, int w, int h, bool fullscreen = false);
    void destroy();

    void setVSync(bool enable);
    void swapBuffers();
    void handleEvents();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    SDL_Window* getNativeHandle() const { return window; }

    Vec2 getSize() const { return Vec2(width, height); }
    float getAspectRatio() const { return (float)width / (float)height; }
};
