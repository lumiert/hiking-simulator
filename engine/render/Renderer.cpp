#include "../render/Renderer.h"

Renderer* Renderer::instance = nullptr;

Renderer::Renderer() : camera(nullptr) {}

Renderer::~Renderer() {}

Renderer& Renderer::getInstance() {
    if (!instance) {
        instance = new Renderer();
    }
    return *instance;
}

void Renderer::initialize() {
    // Renderer initialization
}

void Renderer::submit(MeshPtr mesh, const Mat4& transform, std::shared_ptr<MaterialInstance> material) {
    commands.push_back({mesh, transform, material});
}

void Renderer::render() {
    if (!camera) return;

    for (const auto& cmd : commands) {
        if (cmd.material) {
            cmd.material->bind();
        }

        if (cmd.mesh) {
            cmd.mesh->render();
        }

        if (cmd.material) {
            cmd.material->unbind();
        }
    }
}

void Renderer::clear() {
    commands.clear();
}
