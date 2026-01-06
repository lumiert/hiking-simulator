#include "../environment/Wind.h"

Wind::Wind() : direction(1, 0, 0), strength(0.5f), frequency(0.5f), time(0) {}

void Wind::update(float deltaTime) {
    time += deltaTime * frequency;
    strength = 0.5f + 0.3f * sinf(time);
    direction = glm::normalize(Vec3(sinf(time * 0.5f), 0, cosf(time * 0.5f)));
}
