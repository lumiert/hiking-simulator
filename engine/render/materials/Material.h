#pragma once

#include "../Shader.h"
#include "../Texture.h"
#include "../../math/MathTypes.h"

class Material {
protected:
    std::string name;
    ShaderPtr shader;
    Vec3 albedo;
    float metallic;
    float roughness;
    float aoIntensity;

public:
    Material(const std::string& matName);
    virtual ~Material() = default;

    virtual void bind() const;
    virtual void unbind() const;

    void setShader(ShaderPtr shd) { shader = shd; }
    void setAlbedo(const Vec3& col) { albedo = col; }
    void setMetallic(float m) { metallic = m; }
    void setRoughness(float r) { roughness = r; }
    void setAOIntensity(float ao) { aoIntensity = ao; }

    const std::string& getName() const { return name; }
    ShaderPtr getShader() const { return shader; }
};
