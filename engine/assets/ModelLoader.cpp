#include "../assets/ModelLoader.h"
#include "../render/Mesh.h"
#include "../core/Logger.h"

MeshPtr ModelLoader::loadModel(const std::string& path) {
    LOG_INFO("Loading model: " + path);
    return nullptr;
}

MeshPtr ModelLoader::createQuad(float size) {
    auto mesh = std::make_shared<Mesh>();

    std::vector<Vertex> vertices = {
        {{-size, 0, -size}, {0, 1, 0}, {0, 0}},
        {{size, 0, -size}, {0, 1, 0}, {1, 0}},
        {{size, 0, size}, {0, 1, 0}, {1, 1}},
        {{-size, 0, size}, {0, 1, 0}, {0, 1}}
    };

    std::vector<unsigned int> indices = {0, 1, 2, 0, 2, 3};

    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    return mesh;
}

MeshPtr ModelLoader::createCube(float size) {
    auto mesh = std::make_shared<Mesh>();

    float s = size / 2.0f;
    std::vector<Vertex> vertices = {
        {{-s, -s, -s}, {0, 0, -1}, {0, 0}},
        {{s, -s, -s}, {0, 0, -1}, {1, 0}},
        {{s, s, -s}, {0, 0, -1}, {1, 1}},
        {{-s, s, -s}, {0, 0, -1}, {0, 1}},

        {{-s, -s, s}, {0, 0, 1}, {0, 0}},
        {{s, -s, s}, {0, 0, 1}, {1, 0}},
        {{s, s, s}, {0, 0, 1}, {1, 1}},
        {{-s, s, s}, {0, 0, 1}, {0, 1}},
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 0, 2, 3,
        4, 6, 5, 4, 7, 6,
        0, 4, 5, 0, 5, 1,
        2, 6, 7, 2, 7, 3,
        0, 3, 7, 0, 7, 4,
        1, 5, 6, 1, 6, 2
    };

    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    return mesh;
}

MeshPtr ModelLoader::createPlane(float width, float height, int subdivisions) {
    auto mesh = std::make_shared<Mesh>();

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    float stepX = width / subdivisions;
    float stepY = height / subdivisions;

    for (int y = 0; y <= subdivisions; y++) {
        for (int x = 0; x <= subdivisions; x++) {
            Vertex v;
            v.position = Vec3(x * stepX - width / 2, 0, y * stepY - height / 2);
            v.normal = Vec3(0, 1, 0);
            v.texCoord = Vec2((float)x / subdivisions, (float)y / subdivisions);
            vertices.push_back(v);
        }
    }

    for (int y = 0; y < subdivisions; y++) {
        for (int x = 0; x < subdivisions; x++) {
            int a = y * (subdivisions + 1) + x;
            int b = a + 1;
            int c = a + subdivisions + 1;
            int d = c + 1;

            indices.push_back(a);
            indices.push_back(c);
            indices.push_back(b);

            indices.push_back(b);
            indices.push_back(c);
            indices.push_back(d);
        }
    }

    mesh->setVertices(vertices);
    mesh->setIndices(indices);
    return mesh;
}
