#include <iostream>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>

int main(int, char*[]) {
    std::cout << "[START] Hiking Simulator v1.0\n";
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "[ERROR] SDL Init failed: " << SDL_GetError() << "\n";
        return 1;
    }
    std::cout << "[OK] SDL initialized\n";
    
    // Create window with OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    
    SDL_Window* window = SDL_CreateWindow(
        "Hiking Simulator - OpenGL 3.3",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "[ERROR] SDL Window creation failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }
    std::cout << "[OK] Window created\n";
    
    // Create OpenGL context
    SDL_GLContext ctx = SDL_GL_CreateContext(window);
    if (!ctx) {
        std::cerr << "[ERROR] OpenGL context creation failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    std::cout << "[OK] OpenGL context created\n";
    
    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "[ERROR] GLEW init failed\n";
        SDL_GL_DeleteContext(ctx);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    std::cout << "[OK] GLEW initialized\n";
    
    // Print OpenGL info
    std::cout << "[INFO] OpenGL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "[INFO] GLEW Version: " << glewGetString(GLEW_VERSION) << "\n";
    
    // Setup rendering
    glClearColor(0.15f, 0.20f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    SDL_GL_SetSwapInterval(1);
    
    std::cout << "[INFO] Rendering for 5 seconds...\n";
    
    // Main loop - render for 5 seconds
    bool running = true;
    SDL_Event event;
    int frameCount = 0;
    const int targetFrames = 300; // ~5 sec at 60 FPS
    
    while (running && frameCount < targetFrames) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }
        
        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Animated background
        float t = frameCount / 60.0f;
        glClearColor(
            0.15f + 0.1f * sin(t),
            0.20f + 0.1f * sin(t + 2.0f),
            0.25f + 0.1f * sin(t + 4.0f),
            1.0f
        );
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(window);
        
        frameCount++;
        if (frameCount % 60 == 0) {
            std::cout << "[FRAME] " << frameCount << " frames rendered\n";
        }
    }
    
    std::cout << "[OK] Rendering complete (" << frameCount << " frames)\n";
    
    // Cleanup
    SDL_GL_DeleteContext(ctx);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    std::cout << "[EXIT] Hiking Simulator finished\n";
    return 0;
}
