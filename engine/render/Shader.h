#pragma once

#include <string>
#include <memory>
#include "../math/MathTypes.h"

class Shader {
private:
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

public:
    Shader();
    ~Shader();

    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    bool loadFromStrings(const std::string& vertexCode, const std::string& fragmentCode);

    void use() const;
    unsigned int getProgram() const { return program; }

    void setMat4(const std::string& name, const Mat4& value) const;
    void setMat3(const std::string& name, const Mat3& value) const;
    void setVec3(const std::string& name, const Vec3& value) const;
    void setVec4(const std::string& name, const Vec4& value) const;
    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value) const;

private:
    unsigned int compileShader(const std::string& code, unsigned int type);
    void linkProgram();
};

using ShaderPtr = std::shared_ptr<Shader>;
