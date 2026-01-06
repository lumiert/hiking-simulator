#pragma once

#include <string>
#include <memory>

class Texture;
using TexturePtr = std::shared_ptr<Texture>;

class TextureLoader {
public:
    static TexturePtr loadTexture(const std::string& path);
    static TexturePtr createWhiteTexture();
    static TexturePtr createCheckerTexture(int size = 8);
};
