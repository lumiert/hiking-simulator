#pragma once

#include <SDL2/SDL.h>
#include "../math/MathTypes.h"

enum class KeyState {
    Released,
    Pressed,
    Held
};

class SDLInput {
private:
    static SDLInput* instance;
    const Uint8* keyboardState;
    int numKeys;

    bool mouseButtonPressed[3];
    bool mouseButtonHeld[3];
    Vec2 mousePosition;
    Vec2 mouseDelta;
    float mouseScroll;

    SDLInput();

public:
    ~SDLInput();
    static SDLInput& getInstance();

    void update();

    bool isKeyPressed(SDL_Scancode scancode);
    bool isKeyHeld(SDL_Scancode scancode);

    bool isMouseButtonPressed(int button);
    bool isMouseButtonHeld(int button);
    Vec2 getMousePosition() const { return mousePosition; }
    Vec2 getMouseDelta() const { return mouseDelta; }
    float getMouseScroll() const { return mouseScroll; }

    void setMousePosition(const Vec2& pos);
};
