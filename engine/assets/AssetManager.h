#pragma once

#include <memory>
#include <unordered_map>
#include <string>

class Texture;
class Shader;
class Mesh;

using TexturePtr = std::shared_ptr<Texture>;
using ShaderPtr = std::shared_ptr<Shader>;
using MeshPtr = std::shared_ptr<Mesh>;

class AssetManager {
private:
    static AssetManager* instance;
    std::unordered_map<std::string, TexturePtr> textures;
    std::unordered_map<std::string, ShaderPtr> shaders;
    std::unordered_map<std::string, MeshPtr> meshes;

    AssetManager();

public:
    ~AssetManager();
    static AssetManager& getInstance();

    TexturePtr loadTexture(const std::string& path, const std::string& name = "");
    ShaderPtr loadShader(const std::string& vertPath, const std::string& fragPath, const std::string& name = "");
    MeshPtr loadMesh(const std::string& path, const std::string& name = "");

    TexturePtr getTexture(const std::string& name) const;
    ShaderPtr getShader(const std::string& name) const;
    MeshPtr getMesh(const std::string& name) const;
};
