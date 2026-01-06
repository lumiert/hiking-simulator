#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>

int SDL_main(int, char*[]) {
    std::cout << "[START] Hiking Simulator - SDL2 Test v1.0\n";
    
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "[ERROR] SDL Init failed: " << SDL_GetError() << "\n";
        return 1;
    }
    std::cout << "[✓] SDL2 v" << SDL_MAJOR_VERSION << "." << SDL_MINOR_VERSION << "."
              << SDL_PATCHLEVEL << " initialized\n";
    
    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Hiking Simulator - Test",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        std::cerr << "[ERROR] Window creation failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }
    std::cout << "[✓] Window created (800x600)\n";
    
    // Get window info
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    std::cout << "[✓] Display: " << dm.w << "x" << dm.h << " @ " << dm.refresh_rate << "Hz\n";
    
    // Simulate rendering for 5 seconds
    std::cout << "\n[*] Rendering for 5 seconds...\n\n";
    
    bool running = true;
    int frameCount = 0;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }
        
        // Render (just clear screen)
        SDL_FillRect(SDL_GetWindowSurface(window), nullptr, 
                     SDL_MapRGB(SDL_GetWindowSurface(window)->format, 20, 30, 40));
        SDL_UpdateWindowSurface(window);
        
        frameCount++;
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration<float>(now - startTime).count();
        
        if (frameCount % 60 == 0) {
            float fps = frameCount / elapsed;
            std::cout << "[FRAME] " << frameCount << " | " 
                      << "FPS: " << (int)fps << " | "
                      << "Time: " << (int)elapsed << "s\n";
        }
        
        if (elapsed >= 5.0f) {
            running = false;
        }
        
        SDL_Delay(16);  // ~60 FPS
    }
    
    std::cout << "\n[✓] Rendering complete (" << frameCount << " frames in ~5s)\n";
    
    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    std::cout << "[✓] SDL2 test passed successfully!\n";
    return 0;
}
