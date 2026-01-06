#include "../platform/SDLInput.h"

SDLInput* SDLInput::instance = nullptr;

SDLInput::SDLInput() : keyboardState(nullptr), numKeys(0), mouseScroll(0) {
    for (int i = 0; i < 3; i++) {
        mouseButtonPressed[i] = false;
        mouseButtonHeld[i] = false;
    }
}

SDLInput::~SDLInput() {}

SDLInput& SDLInput::getInstance() {
    if (!instance) {
        instance = new SDLInput();
    }
    return *instance;
}

void SDLInput::update() {
    keyboardState = SDL_GetKeyboardState(&numKeys);
    mouseDelta = Vec2(0, 0);
    mouseScroll = 0;

    for (int i = 0; i < 3; i++) {
        mouseButtonPressed[i] = false;
    }

    int mouseX, mouseY;
    Uint32 buttons = SDL_GetMouseState(&mouseX, &mouseY);

    Vec2 newMousePos(mouseX, mouseY);
    mouseDelta = newMousePos - mousePosition;
    mousePosition = newMousePos;

    for (int i = 0; i < 3; i++) {
        bool currentHeld = (buttons & SDL_BUTTON(i + 1)) != 0;
        if (currentHeld && !mouseButtonHeld[i]) {
            mouseButtonPressed[i] = true;
        }
        mouseButtonHeld[i] = currentHeld;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEWHEEL) {
            mouseScroll = event.wheel.y > 0 ? 1.0f : -1.0f;
        }
    }
}

bool SDLInput::isKeyPressed(SDL_Scancode scancode) {
    return keyboardState && keyboardState[scancode];
}

bool SDLInput::isKeyHeld(SDL_Scancode scancode) {
    return keyboardState && keyboardState[scancode];
}

bool SDLInput::isMouseButtonPressed(int button) {
    return button >= 0 && button < 3 && mouseButtonPressed[button];
}

bool SDLInput::isMouseButtonHeld(int button) {
    return button >= 0 && button < 3 && mouseButtonHeld[button];
}

void SDLInput::setMousePosition(const Vec2& pos) {
    SDL_WarpMouseGlobal((int)pos.x, (int)pos.y);
    mousePosition = pos;
}
