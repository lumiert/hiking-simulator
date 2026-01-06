#include "engine/core/Logger.h"
#include "engine/core/Timer.h"
#include "engine/platform/Time.h"
#include <memory>
#include <string>

int main(int argc, char* argv[]) {
    try {
        Logger::getInstance().init("hiking_simulator.log");
        Logger::getInstance().setMinLevel(LogLevel::Debug);

        LOG_INFO("======================================");
        LOG_INFO("Hiking Simulator - Core Modules");
        LOG_INFO("======================================");

        // Initialize Timer
        auto timer = std::make_unique<Timer>();
        if (!timer) {
            LOG_ERROR("Failed to create timer");
            return -1;
        }
        timer->start();
        LOG_INFO("Timer initialized");

        // Initialize Time system
        Time::init();
        LOG_INFO("Time system initialized");

        // Simulate game loop
        LOG_INFO("Simulating 5 frames...");
        for (int i = 0; i < 5; ++i) {
            timer->tick();
            float deltaTime = timer->getDeltaTime();
            Time::update(deltaTime);
            
            LOG_INFO("Frame " + std::to_string(i + 1) + 
                     " - DT: " + std::to_string(deltaTime) + 
                     "s, Total: " + std::to_string(Time::getTotalTime()) + "s");
        }

        LOG_INFO("Application terminated successfully");
        return 0;
    } catch (const std::exception& e) {
        LOG_CRITICAL(std::string("Fatal error: ") + e.what());
        return -1;
    }
}
