#pragma once

#include <string>
#include <memory>

class Texture {
private:
    unsigned int handle;
    int width, height;
    int channels;

public:
    Texture();
    ~Texture();

    bool loadFromFile(const std::string& path);
    void bind(unsigned int slot = 0) const;
    void unbind() const;

    unsigned int getHandle() const { return handle; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

using TexturePtr = std::shared_ptr<Texture>;
