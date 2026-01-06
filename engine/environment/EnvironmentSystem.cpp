#include "../environment/EnvironmentSystem.h"

EnvironmentSystem::EnvironmentSystem()
    : wind(std::make_unique<Wind>()),
      sunDirection(-0.3f, -1.0f, -0.5f),
      sunColor(1.0f, 1.0f, 0.9f),
      ambientLight(0.4f) {
    sunDirection = glm::normalize(sunDirection);
}

void EnvironmentSystem::update(float deltaTime) {
    wind->update(deltaTime);
}
