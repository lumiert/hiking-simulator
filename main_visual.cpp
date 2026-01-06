#include "engine/core/Logger.h"
#include "engine/core/Timer.h"
#include "engine/platform/Time.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

class ConsoleUI {
private:
    int width;
    int height;
    float rotation;
    float scale;

public:
    ConsoleUI(int w = 80, int h = 24) : width(w), height(h), rotation(0), scale(1.0f) {}

    void drawBox(int x, int y, int w, int h, const std::string& title) {
        std::cout << "\x1b[" << (y + 1) << ";" << (x + 1) << "H";
        std::cout << "\x1b[36m┌";
        for (int i = 0; i < w - 2; ++i) std::cout << "─";
        std::cout << "┐\x1b[0m";

        for (int i = 1; i < h - 1; ++i) {
            std::cout << "\x1b[" << (y + i + 1) << ";" << (x + 1) << "H";
            std::cout << "\x1b[36m│\x1b[0m";
            std::cout << "\x1b[" << (y + i + 1) << ";" << (x + w) << "H";
            std::cout << "\x1b[36m│\x1b[0m";
        }

        std::cout << "\x1b[" << (y + h) << ";" << (x + 1) << "H";
        std::cout << "\x1b[36m└";
        for (int i = 0; i < w - 2; ++i) std::cout << "─";
        std::cout << "┘\x1b[0m";

        // Title
        if (!title.empty() && title.length() < w - 2) {
            int titleX = x + (w - title.length()) / 2;
            std::cout << "\x1b[" << (y + 1) << ";" << titleX << "H";
            std::cout << "\x1b[33m" << title << "\x1b[0m";
        }
    }

    void drawBar(int x, int y, int width, float value, const std::string& label) {
        int filled = (int)(width * std::max(0.0f, std::min(1.0f, value)));
        
        std::cout << "\x1b[" << (y + 1) << ";" << (x + 1) << "H";
        std::cout << "\x1b[33m" << label << ": \x1b[0m";
        std::cout << "\x1b[32m[";
        for (int i = 0; i < filled; ++i) std::cout << "█";
        for (int i = filled; i < width; ++i) std::cout << "░";
        std::cout << "]\x1b[0m";
        std::cout << " " << (int)(value * 100) << "%";
    }

    void drawScene(float fps, float totalTime) {
        CLEAR_SCREEN();
        
        // Header
        std::cout << "\x1b[1;32m";
        std::cout << "╔════════════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║         🏔️  HIKING SIMULATOR - VISUAL INTERFACE 🏔️                          ║\n";
        std::cout << "║                     WinLibs MinGW64 - C++17                                    ║\n";
        std::cout << "╚════════════════════════════════════════════════════════════════════════════════╝\n";
        std::cout << "\x1b[0m\n";

        // Main scene area
        std::cout << "\x1b[36m";
        std::cout << "┌──────────────────────────────────────────────────────────────────────────────────┐\n";
        
        // Draw mountain scene
        int mountainHeight = 15;
        for (int row = 0; row < mountainHeight; ++row) {
            std::cout << "│ ";
            
            // Sky
            if (row < mountainHeight / 2) {
                for (int col = 0; col < 78; ++col) {
                    if (col % 8 == (int)(rotation) % 8) {
                        std::cout << "\x1b[34m✦\x1b[36m";
                    } else {
                        std::cout << " ";
                    }
                }
            } else {
                // Mountains
                int mountainPos = (int)(40 + 15 * std::sin(rotation * 0.05f));
                for (int col = 0; col < 78; ++col) {
                    int distToMountain = col - mountainPos;
                    int height = (int)(10 * std::cos(distToMountain * 0.1f) + 5);
                    
                    if (row >= mountainHeight - height) {
                        std::cout << "\x1b[33m▲\x1b[36m";
                    } else if (row == mountainHeight - height - 1) {
                        std::cout << "\x1b[32m~\x1b[36m";
                    } else {
                        std::cout << " ";
                    }
                }
            }
            std::cout << " │\n";
        }
        
        std::cout << "└──────────────────────────────────────────────────────────────────────────────────┘\n";
        std::cout << "\x1b[0m\n";

        // Statistics panel
        std::cout << "\x1b[36m┌─ Statistics ";
        for (int i = 0; i < 62; ++i) std::cout << "─";
        std::cout << "┐\x1b[0m\n";

        std::cout << "\x1b[32m│\x1b[0m";
        std::cout << "  FPS: \x1b[33m" << (int)fps << "\x1b[0m";
        std::cout << "  │  Total Time: \x1b[33m" << (int)totalTime << "ms\x1b[0m";
        std::cout << "  │  Camera Position: (0.0, 2.0, 5.0)";
        std::cout << "  \x1b[32m│\x1b[0m\n";

        // Progress bars
        std::cout << "\x1b[36m│\x1b[0m";
        std::cout << "\x1b[33m Memory Usage:\x1b[0m ";
        float memUsage = 0.35f + 0.15f * std::sin(rotation * 0.02f);
        for (int i = 0; i < 20; ++i) {
            if (i < (int)(memUsage * 20)) {
                std::cout << "\x1b[31m█\x1b[0m";
            } else {
                std::cout << "\x1b[32m░\x1b[0m";
            }
        }
        std::cout << " " << (int)(memUsage * 100) << "%";
        std::cout << "                  \x1b[32m│\x1b[0m\n";

        std::cout << "\x1b[36m│\x1b[0m";
        std::cout << "\x1b[33m Physics Updates:\x1b[0m ";
        float physics = 0.75f;
        for (int i = 0; i < 20; ++i) {
            if (i < (int)(physics * 20)) {
                std::cout << "\x1b[32m█\x1b[0m";
            } else {
                std::cout << "\x1b[32m░\x1b[0m";
            }
        }
        std::cout << " " << (int)(physics * 100) << "%";
        std::cout << "                \x1b[32m│\x1b[0m\n";

        std::cout << "\x1b[36m└";
        for (int i = 0; i < 76; ++i) std::cout << "─";
        std::cout << "┘\x1b[0m\n\n";

        // Info panel
        std::cout << "\x1b[1;35m";
        std::cout << "  [SPACE] - Toggle Animation  [ESC] - Exit  [←/→] - Rotate View\n";
        std::cout << "  [+/-] - Zoom  [R] - Reset  [P] - Pause\n";
        std::cout << "\x1b[0m";
    }

    void update() {
        rotation += 1.0f;
    }

    float getRotation() const { return rotation; }
};

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().init("hiking_simulator.log");
        Logger::getInstance().setMinLevel(LogLevel::Debug);

        LOG_INFO("======================================");
        LOG_INFO("Hiking Simulator - Visual Interface");
        LOG_INFO("======================================");

        auto timer = std::make_unique<Timer>();
        timer->start();
        Time::init();

        LOG_INFO("Visual interface started");

        ConsoleUI ui(80, 24);
        
        LOG_INFO("Running visual demo for 30 seconds...");

        auto startTime = std::chrono::high_resolution_clock::now();
        int frameCount = 0;

        // Run for 30 seconds or until user interrupts
        while (true) {
            auto now = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                now - startTime).count();

            if (elapsed >= 30) break;

            timer->tick();
            float deltaTime = timer->getDeltaTime();
            Time::update(deltaTime);
            ui.update();

            float fps = 1.0f / deltaTime;
            ui.drawScene(fps, Time::getTotalTime() * 1000);

            frameCount++;

            // ~30 FPS
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
        }

        LOG_INFO("Visual demo complete. Frames rendered: " + std::to_string(frameCount));
        LOG_INFO("Application terminated successfully");
        
        std::cout << "\n\x1b[32m✓ Simulation completed successfully!\x1b[0m\n\n";
        return 0;

    } catch (const std::exception& e) {
        LOG_CRITICAL(std::string("Fatal error: ") + e.what());
        std::cerr << "\x1b[31mError: " << e.what() << "\x1b[0m\n";
        return -1;
    }
}
