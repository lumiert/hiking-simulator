#pragma once

#include "Material.h"
#include <memory>

class MaterialInstance {
private:
    std::shared_ptr<Material> material;
    std::unordered_map<std::string, TexturePtr> textures;

public:
    MaterialInstance(std::shared_ptr<Material> mat);

    void bind() const;
    void unbind() const;

    void setTexture(const std::string& slot, TexturePtr texture);
    TexturePtr getTexture(const std::string& slot) const;

    std::shared_ptr<Material> getMaterial() const { return material; }
};
