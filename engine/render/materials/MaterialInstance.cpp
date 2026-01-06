#include "../render/materials/MaterialInstance.h"

MaterialInstance::MaterialInstance(std::shared_ptr<Material> mat) : material(mat) {}

void MaterialInstance::bind() const {
    if (material) {
        material->bind();
    }

    for (const auto& pair : textures) {
        if (pair.second) {
            pair.second->bind();
        }
    }
}

void MaterialInstance::unbind() const {
    if (material) {
        material->unbind();
    }

    for (const auto& pair : textures) {
        if (pair.second) {
            pair.second->unbind();
        }
    }
}

void MaterialInstance::setTexture(const std::string& slot, TexturePtr texture) {
    textures[slot] = texture;
}

TexturePtr MaterialInstance::getTexture(const std::string& slot) const {
    auto it = textures.find(slot);
    return it != textures.end() ? it->second : nullptr;
}
