#pragma once

#include <string>
#include <memory>

// Forward declare Mesh for ModelLoader
class Mesh;
using MeshPtr = std::shared_ptr<Mesh>;

class ModelLoader {
public:
    static MeshPtr loadModel(const std::string& path);
    static MeshPtr createQuad(float size = 1.0f);
    static MeshPtr createCube(float size = 1.0f);
    static MeshPtr createPlane(float width, float height, int subdivisions = 1);
};
