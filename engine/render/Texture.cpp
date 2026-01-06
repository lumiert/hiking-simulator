#include "../render/Texture.h"
#include "../core/Logger.h"
#include <GL/glew.h>
#include <stb_image.h>

Texture::Texture() : handle(0), width(0), height(0), channels(0) {}

Texture::~Texture() {
    if (handle) glDeleteTextures(1, &handle);
}

bool Texture::loadFromFile(const std::string& path) {
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (!data) {
        LOG_ERROR("Failed to load texture: " + path);
        return false;
    }

    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);

    GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);
    LOG_INFO("Texture loaded: " + path);
    return true;
}

void Texture::bind(unsigned int slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture::unbind() const {
    glBindTexture(GL_TEXTURE_2D, 0);
}
