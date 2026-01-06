#include "Timer.h"

Timer::Timer() : deltaTime(0.0f), totalTime(0.0f) {
    startTime = Clock::now();
    lastFrameTime = startTime;
    currentFrameTime = startTime;
}

void Timer::start() {
    startTime = Clock::now();
    lastFrameTime = startTime;
    currentFrameTime = startTime;
}

void Timer::tick() {
    lastFrameTime = currentFrameTime;
    currentFrameTime = Clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        currentFrameTime - lastFrameTime
    );
    deltaTime = duration.count() / 1000000.0f;

    auto totalDuration = std::chrono::duration_cast<std::chrono::microseconds>(
        currentFrameTime - startTime
    );
    totalTime = totalDuration.count() / 1000000.0f;
}

float Timer::getDeltaTime() const {
    return deltaTime;
}

float Timer::getTotalTime() const {
    return totalTime;
}

float Timer::getFPS() const {
    return deltaTime > 0 ? 1.0f / deltaTime : 0.0f;
}
