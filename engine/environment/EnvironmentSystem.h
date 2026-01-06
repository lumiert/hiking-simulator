#pragma once

#include "Wind.h"
#include <memory>

class EnvironmentSystem {
private:
    std::unique_ptr<Wind> wind;
    Vec3 sunDirection;
    Vec3 sunColor;
    float ambientLight;

public:
    EnvironmentSystem();

    void update(float deltaTime);

    Wind& getWind() { return *wind; }
    Vec3 getSunDirection() const { return sunDirection; }
    Vec3 getSunColor() const { return sunColor; }
    float getAmbientLight() const { return ambientLight; }

    void setSunDirection(const Vec3& dir) { sunDirection = glm::normalize(dir); }
    void setSunColor(const Vec3& color) { sunColor = color; }
    void setAmbientLight(float light) { ambientLight = light; }
};
