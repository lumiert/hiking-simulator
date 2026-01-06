#pragma once

#include <vector>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color;
};

class PlaneGenerator {
public:
    static std::vector<Vertex> generatePlane(float width, float height, int subdivisions = 10) {
        std::vector<Vertex> vertices;
        
        float stepX = width / subdivisions;
        float stepZ = height / subdivisions;
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;
        
        // Gerar grid de vértices
        for (int z = 0; z <= subdivisions; ++z) {
            for (int x = 0; x <= subdivisions; ++x) {
                float posX = -halfWidth + x * stepX;
                float posZ = -halfHeight + z * stepZ;
                
                // Cor baseada na posição (para visual interessante)
                float colorR = 0.2f + (x / (float)subdivisions) * 0.3f;
                float colorG = 0.4f + (z / (float)subdivisions) * 0.3f;
                float colorB = 0.3f;
                
                Vertex v;
                v.position = glm::vec3(posX, 0.0f, posZ);
                v.color = glm::vec3(colorR, colorG, colorB);
                
                vertices.push_back(v);
            }
        }
        
        return vertices;
    }
    
    static std::vector<unsigned int> generatePlaneIndices(int subdivisions = 10) {
        std::vector<unsigned int> indices;
        
        int verticesPerRow = subdivisions + 1;
        
        // Gerar triângulos
        for (int z = 0; z < subdivisions; ++z) {
            for (int x = 0; x < subdivisions; ++x) {
                unsigned int topLeft = z * verticesPerRow + x;
                unsigned int topRight = topLeft + 1;
                unsigned int bottomLeft = (z + 1) * verticesPerRow + x;
                unsigned int bottomRight = bottomLeft + 1;
                
                // Primeiro triângulo
                indices.push_back(topLeft);
                indices.push_back(bottomLeft);
                indices.push_back(topRight);
                
                // Segundo triângulo
                indices.push_back(topRight);
                indices.push_back(bottomLeft);
                indices.push_back(bottomRight);
            }
        }
        
        return indices;
    }
};
