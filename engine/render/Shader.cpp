#include "../render/Shader.h"
#include "../core/FileSystem.h"
#include "../core/Logger.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader() : program(0), vertexShader(0), fragmentShader(0) {}

Shader::~Shader() {
    if (program) glDeleteProgram(program);
    if (vertexShader) glDeleteShader(vertexShader);
    if (fragmentShader) glDeleteShader(fragmentShader);
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = FileSystem::readTextFile(vertexPath);
    std::string fragmentCode = FileSystem::readTextFile(fragmentPath);
    return loadFromStrings(vertexCode, fragmentCode);
}

bool Shader::loadFromStrings(const std::string& vertexCode, const std::string& fragmentCode) {
    vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER);
    if (!vertexShader) {
        LOG_ERROR("Failed to compile vertex shader");
        return false;
    }

    fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER);
    if (!fragmentShader) {
        LOG_ERROR("Failed to compile fragment shader");
        glDeleteShader(vertexShader);
        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    linkProgram();

    return true;
}

void Shader::use() const {
    glUseProgram(program);
}

unsigned int Shader::compileShader(const std::string& code, unsigned int type) {
    unsigned int shader = glCreateShader(type);
    const char* codePtr = code.c_str();
    glShaderSource(shader, 1, &codePtr, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        LOG_ERROR("Shader compilation failed: " + std::string(infoLog));
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void Shader::linkProgram() {
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        LOG_ERROR("Shader linking failed: " + std::string(infoLog));
    }
}

void Shader::setMat4(const std::string& name, const Mat4& value) const {
    GLint loc = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string& name, const Mat3& value) const {
    GLint loc = glGetUniformLocation(program, name.c_str());
    glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, const Vec3& value) const {
    GLint loc = glGetUniformLocation(program, name.c_str());
    glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, const Vec4& value) const {
    GLint loc = glGetUniformLocation(program, name.c_str());
    glUniform4fv(loc, 1, glm::value_ptr(value));
}

void Shader::setFloat(const std::string& name, float value) const {
    GLint loc = glGetUniformLocation(program, name.c_str());
    glUniform1f(loc, value);
}

void Shader::setInt(const std::string& name, int value) const {
    GLint loc = glGetUniformLocation(program, name.c_str());
    glUniform1i(loc, value);
}
