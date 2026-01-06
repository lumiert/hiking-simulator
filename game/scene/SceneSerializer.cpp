#include "../scene/SceneSerializer.h"
#include "../../engine/core/FileSystem.h"

bool SceneSerializer::saveScene(const std::string& filename, const WorldPtr& world) {
    // Implement JSON serialization for scenes
    return true;
}

WorldPtr SceneSerializer::loadScene(const std::string& filename) {
    // Implement JSON deserialization for scenes
    return nullptr;
}
