#pragma once

#include "../math/MathTypes.h"

class Wind {
private:
    Vec3 direction;
    float strength;
    float frequency;
    float time;

public:
    Wind();

    void update(float deltaTime);

    Vec3 getDirection() const { return direction; }
    float getStrength() const { return strength; }
    Vec3 getForce() const { return direction * strength; }

    void setDirection(const Vec3& dir) { direction = glm::normalize(dir); }
    void setStrength(float s) { strength = s; }
};
