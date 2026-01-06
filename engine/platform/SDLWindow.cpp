#include "../platform/SDLWindow.h"
#include "../core/Logger.h"
#include <GL/glew.h>

SDLWindow::SDLWindow() : window(nullptr), width(0), height(0), vsync(true) {}

SDLWindow::~SDLWindow() {
    destroy();
}

bool SDLWindow::create(const std::string& title, int w, int h, bool fullscreen) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        LOG_ERROR("SDL initialization failed");
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_CORE_PROFILE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    int flags = SDL_WINDOW_OPENGL;
    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        w, h,
        flags
    );

    if (!window) {
        LOG_ERROR("Failed to create SDL window");
        return false;
    }

    width = w;
    height = h;

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        LOG_ERROR("Failed to create OpenGL context");
        return false;
    }

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG_ERROR("GLEW initialization failed");
        return false;
    }

    SDL_GL_SetSwapInterval(vsync ? 1 : 0);

    LOG_INFO("SDL Window created: " + std::to_string(w) + "x" + std::to_string(h));
    return true;
}

void SDLWindow::destroy() {
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void SDLWindow::setVSync(bool enable) {
    vsync = enable;
    if (window) {
        SDL_GL_SetSwapInterval(enable ? 1 : 0);
    }
}

void SDLWindow::swapBuffers() {
    if (window) {
        SDL_GL_SwapWindow(window);
    }
}

void SDLWindow::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                break;
            case SDL_MOUSEMOTION:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                break;
        }
    }
}
