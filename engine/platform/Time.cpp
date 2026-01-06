#include "../platform/Time.h"

float Time::deltaTime = 0.016f;
float Time::totalTime = 0.0f;
uint64_t Time::frameCount = 0;

void Time::init() {
    deltaTime = 0.016f;
    totalTime = 0.0f;
    frameCount = 0;
}

void Time::update(float dt) {
    deltaTime = dt;
    totalTime += dt;
    frameCount++;
}
