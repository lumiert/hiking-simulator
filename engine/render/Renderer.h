#pragma once

#include "../math/Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "materials/MaterialInstance.h"
#include <memory>
#include <vector>

struct RenderCommand {
    MeshPtr mesh;
    Mat4 transform;
    std::shared_ptr<MaterialInstance> material;
};

class Renderer {
private:
    static Renderer* instance;
    CameraPtr camera;
    std::vector<RenderCommand> commands;

    Renderer();

public:
    ~Renderer();
    static Renderer& getInstance();

    void initialize();
    void setCamera(CameraPtr cam) { camera = cam; }

    void submit(MeshPtr mesh, const Mat4& transform, std::shared_ptr<MaterialInstance> material);
    void render();
    void clear();

    CameraPtr getCamera() const { return camera; }
};
