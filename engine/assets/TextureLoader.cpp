#include "../assets/TextureLoader.h"
#include "../render/Texture.h"
#include <GL/glew.h>

TexturePtr TextureLoader::loadTexture(const std::string& path) {
    auto texture = std::make_shared<Texture>();
    if (texture->loadFromFile(path)) {
        return texture;
    }
    return nullptr;
}

TexturePtr TextureLoader::createWhiteTexture() {
    auto texture = std::make_shared<Texture>();

    unsigned char white[] = {255, 255, 255, 255};
    unsigned int handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, white);

    return texture;
}

TexturePtr TextureLoader::createCheckerTexture(int size) {
    auto texture = std::make_shared<Texture>();

    std::vector<unsigned char> data(size * size * 4);
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            bool white = ((x / 4) + (y / 4)) % 2 == 0;
            unsigned char color = white ? 255 : 0;

            int idx = (y * size + x) * 4;
            data[idx] = color;
            data[idx + 1] = color;
            data[idx + 2] = color;
            data[idx + 3] = 255;
        }
    }

    unsigned int handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

    return texture;
}
