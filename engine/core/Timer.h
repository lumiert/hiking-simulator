#pragma once

#include <chrono>

class Timer {
private:
    using Clock = std::chrono::high_resolution_clock;
    using Timepoint = std::chrono::time_point<Clock>;

    Timepoint startTime;
    Timepoint lastFrameTime;
    Timepoint currentFrameTime;
    float deltaTime;
    float totalTime;

public:
    Timer();

    void start();
    void tick();

    float getDeltaTime() const;
    float getTotalTime() const;
    float getFPS() const;
};
