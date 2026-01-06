#include "../render/materials/Material.h"

Material::Material(const std::string& matName)
    : name(matName), shader(nullptr),
      albedo(0.8f), metallic(0.0f), roughness(0.5f), aoIntensity(1.0f) {}

void Material::bind() const {
    if (shader) {
        shader->use();
        shader->setVec3("uAlbedo", albedo);
        shader->setFloat("uMetallic", metallic);
        shader->setFloat("uRoughness", roughness);
        shader->setFloat("uAOIntensity", aoIntensity);
    }
}

void Material::unbind() const {}
