#include "../assets/AssetManager.h"
#include "ModelLoader.h"
#include "TextureLoader.h"

AssetManager* AssetManager::instance = nullptr;

AssetManager::AssetManager() {}

AssetManager::~AssetManager() {}

AssetManager& AssetManager::getInstance() {
    if (!instance) {
        instance = new AssetManager();
    }
    return *instance;
}

TexturePtr AssetManager::loadTexture(const std::string& path, const std::string& name) {
    auto texture = TextureLoader::loadTexture(path);
    if (texture) {
        std::string key = name.empty() ? path : name;
        textures[key] = texture;
    }
    return texture;
}

ShaderPtr AssetManager::loadShader(const std::string& vertPath, const std::string& fragPath, const std::string& name) {
    auto shader = std::make_shared<Shader>();
    if (shader->loadFromFiles(vertPath, fragPath)) {
        std::string key = name.empty() ? vertPath + fragPath : name;
        shaders[key] = shader;
        return shader;
    }
    return nullptr;
}

MeshPtr AssetManager::loadMesh(const std::string& path, const std::string& name) {
    auto mesh = ModelLoader::loadModel(path);
    if (mesh) {
        std::string key = name.empty() ? path : name;
        meshes[key] = mesh;
    }
    return mesh;
}

TexturePtr AssetManager::getTexture(const std::string& name) const {
    auto it = textures.find(name);
    return it != textures.end() ? it->second : nullptr;
}

ShaderPtr AssetManager::getShader(const std::string& name) const {
    auto it = shaders.find(name);
    return it != shaders.end() ? it->second : nullptr;
}

MeshPtr AssetManager::getMesh(const std::string& name) const {
    auto it = meshes.find(name);
    return it != meshes.end() ? it->second : nullptr;
}
