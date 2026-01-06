#pragma once

#include <chrono>

class Time {
private:
    static float deltaTime;
    static float totalTime;
    static uint64_t frameCount;

public:
    static void init();
    static void update(float dt);

    static float getDeltaTime() { return deltaTime; }
    static float getTotalTime() { return totalTime; }
    static uint64_t getFrameCount() { return frameCount; }
    static float getFPS() { return 1.0f / deltaTime; }
};
