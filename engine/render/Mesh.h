#pragma once

#include <vector>
#include <memory>
#include "../math/MathTypes.h"

struct Vertex {
    Vec3 position;
    Vec3 normal;
    Vec2 texCoord;
};

class Mesh {
private:
    unsigned int VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

public:
    Mesh();
    ~Mesh();

    void setVertices(const std::vector<Vertex>& verts);
    void setIndices(const std::vector<unsigned int>& inds);
    void setupMesh();

    void render() const;

    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }
};

using MeshPtr = std::shared_ptr<Mesh>;
