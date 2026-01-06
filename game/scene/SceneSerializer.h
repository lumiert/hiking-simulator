#pragma once

#include "../../engine/scene/World.h"
#include "../../engine/environment/EnvironmentSystem.h"
#include "../../engine/render/Renderer.h"
#include <memory>
#include <string>

class SceneSerializer {
public:
    static bool saveScene(const std::string& filename, const WorldPtr& world);
    static WorldPtr loadScene(const std::string& filename);
};
