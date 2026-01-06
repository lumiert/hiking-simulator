#include <iostream>
#include <memory>
#include <vector>
#include <chrono>
#include <cstdio>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

class GraphicsApp {
private:
    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;
    bool running = true;
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

public:
    GraphicsApp() = default;
    ~GraphicsApp() {
        cleanup();
    }

    bool initialize() {
        std::cout << "[INFO] Initializing Graphics Application...\n";
        
        // Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "[ERROR] Failed to initialize SDL: " << SDL_GetError() << "\n";
            return false;
        }

        // Create window
        window = SDL_CreateWindow(
            "Hiking Simulator - OpenGL",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );

        if (!window) {
            std::cerr << "[ERROR] Failed to create SDL window: " << SDL_GetError() << "\n";
            SDL_Quit();
            return false;
        }

        // Create OpenGL context
        glContext = SDL_GL_CreateContext(window);
        if (!glContext) {
            std::cerr << "[ERROR] Failed to create OpenGL context: " << SDL_GetError() << "\n";
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        // Initialize GLEW
        glewExperimental = GL_TRUE;
        GLenum glewStatus = glewInit();
        if (glewStatus != GLEW_OK) {
            std::cerr << "[ERROR] GLEW initialization failed\n";
            SDL_GL_DeleteContext(glContext);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return false;
        }

        // Setup OpenGL
        SDL_GL_SetSwapInterval(1); // vsync
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glEnable(GL_DEPTH_TEST);

        std::cout << "[INFO] Graphics initialized successfully!\n";
        std::cout << "[INFO] OpenGL Version: " << (const char*)glGetString(GL_VERSION) << "\n";
        std::cout << "[INFO] GLEW Version: " << (const char*)glewGetString(GLEW_VERSION) << "\n";

        return true;
    }

    void run() {
        auto frameStart = std::chrono::high_resolution_clock::now();
        float frameTime = 0.0f;
        int frameCount = 0;
        const int MAX_FRAMES = 300; // 5 seconds at 60 FPS

        while (running && frameCount < MAX_FRAMES) {
            frameStart = std::chrono::high_resolution_clock::now();

            // Handle events
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDL_KEYDOWN:
                        if (event.key.keysym.sym == SDLK_ESCAPE) {
                            running = false;
                        }
                        break;
                }
            }

            // Clear screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Simple rendering test - draw gradient
            drawFrame(frameCount);

            // Swap buffers
            SDL_GL_SwapWindow(window);

            auto frameEnd = std::chrono::high_resolution_clock::now();
            frameTime = std::chrono::duration<float>(frameEnd - frameStart).count();
            frameCount++;

            if (frameCount % 60 == 0) {
                float fps = 1.0f / frameTime;
                std::cout << "[INFO] Frame " << frameCount << " - FPS: " << (int)fps << "\n";
            }
        }

        std::cout << "[INFO] Application finished. Total frames rendered: " << frameCount << "\n";
    }

    void drawFrame(int frameCount) {
        // Simple animated test - change background color
        float angle = (frameCount / 60.0f) * 3.14159f * 2.0f;
        float r = 0.3f + 0.2f * sin(angle);
        float g = 0.3f + 0.2f * sin(angle + 2.0f);
        float b = 0.3f + 0.2f * sin(angle + 4.0f);
        
        glClearColor(r, g, b, 1.0f);
    }

private:
    void cleanup() {
        if (glContext) {
            SDL_GL_DeleteContext(glContext);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }
};

int main(int, char*[]) {
    std::cout << "=== Hiking Simulator - Graphics Version ===\n";

    GraphicsApp app;
    if (!app.initialize()) {
        std::cerr << "[ERROR] Failed to initialize application\n";
        return 1;
    }

    app.run();

    std::cout << "[INFO] Application terminated normally\n";
    return 0;
}
