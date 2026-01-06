#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

// Collision types
enum class CollisionType {
    NONE = 0,
    STATIC = 1,
    DYNAMIC = 3
};

// Simple GLTF mesh data
struct GLTFMeshData {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    GLuint VAO = 0, VBO = 0, EBO = 0;
    
    void setupGL() {
        if (positions.empty() || indices.empty()) return;
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        // Combine position and normal data into single vertex buffer
        std::vector<float> vertexData;
        for (size_t i = 0; i < positions.size(); ++i) {
            vertexData.push_back(positions[i].x);
            vertexData.push_back(positions[i].y);
            vertexData.push_back(positions[i].z);
            
            if (i < normals.size()) {
                vertexData.push_back(normals[i].x);
                vertexData.push_back(normals[i].y);
                vertexData.push_back(normals[i].z);
            } else {
                vertexData.push_back(0.0f);
                vertexData.push_back(1.0f);
                vertexData.push_back(0.0f);
            }
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        
        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    
    void render() const {
        if (VAO == 0) return;
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    
    void cleanup() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
    }
};

// 3D Object in scene
struct SceneObject {
    int id;
    std::string modelPath;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    CollisionType collisionType;
    GLTFMeshData mesh;
    
    SceneObject(int id_, const std::string& path, const glm::vec3& pos, CollisionType col)
        : id(id_), modelPath(path), position(pos), rotation(0.0f), scale(1.0f), collisionType(col) {}
    
    glm::mat4 getModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }
};

// Scene manager - handles object placement and rendering
class SceneManager {
private:
    std::vector<SceneObject> objects;
    std::map<std::string, GLTFMeshData> meshCache;
    int nextObjectId = 1;
    
public:
    SceneManager() = default;
    
    ~SceneManager() {
        cleanup();
    }
    
    int placeObject(const std::string& modelPath, float x, float y, float z, int collisionType) {
        return placeObject(modelPath, x, y, z, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, collisionType);
    }
    
    int placeObject(const std::string& modelPath, 
                   float x, float y, float z,
                   float rotX, float rotY, float rotZ,
                   float scaleX, float scaleY, float scaleZ,
                   int collisionType) {
        glm::vec3 pos(x, y, z);
        glm::vec3 rot(rotX, rotY, rotZ);
        glm::vec3 scl(scaleX, scaleY, scaleZ);
        CollisionType colType = static_cast<CollisionType>(collisionType);
        
        SceneObject obj(nextObjectId++, modelPath, pos, colType);
        obj.rotation = rot;
        obj.scale = scl;
        
        // Load or use cached mesh
        if (meshCache.find(modelPath) == meshCache.end()) {
            std::cout << "[*] Loading model: " << modelPath << "\n";
            GLTFMeshData mesh = loadGLTF(modelPath);
            mesh.setupGL();
            meshCache[modelPath] = mesh;
        }
        
        obj.mesh = meshCache[modelPath];
        objects.push_back(obj);
        
        std::cout << "[OK] Object #" << (nextObjectId - 1) << " placed at (" 
                  << x << ", " << y << ", " << z << ")"
                  << " Rot: (" << rotX << ", " << rotY << ", " << rotZ << ")"
                  << " Scale: (" << scaleX << ", " << scaleY << ", " << scaleZ << ")"
                  << " Collision: " << collisionType << "\n";
        
        return nextObjectId - 1;
    }
    
    void renderAll(GLuint shaderProgram, const glm::mat4& view, const glm::mat4& projection, float time) {
        glUseProgram(shaderProgram);
        
        GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
        GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
        GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
        GLint timeLoc = glGetUniformLocation(shaderProgram, "time");
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(timeLoc, time);
        
        for (auto& obj : objects) {
            glm::mat4 modelMat = obj.getModelMatrix();
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
            obj.mesh.render();
        }
    }
    
    SceneObject* getObject(int id) {
        for (auto& obj : objects) {
            if (obj.id == id) return &obj;
        }
        return nullptr;
    }
    
    void removeObject(int id) {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
            [id](const SceneObject& obj) { return obj.id == id; }), objects.end());
    }
    
    void cleanup() {
        for (auto& [path, mesh] : meshCache) {
            mesh.cleanup();
        }
        objects.clear();
        meshCache.clear();
    }
    
private:
    GLTFMeshData loadGLTF(const std::string& filePath) {
        GLTFMeshData mesh;
        
        // Read GLTF JSON file
        std::ifstream file(filePath);
        if (!file.is_open()) {
            std::cerr << "[ERROR] Could not open GLTF file: " << filePath << "\n";
            return createDefaultCube();
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        file.close();
        
        // Very basic JSON parsing for GLTF
        // Look for binary buffer reference
        size_t binPos = content.find("\"uri\":");
        if (binPos == std::string::npos) {
            // Try embedded data
            return createDefaultCube();
        }
        
        // Extract bin filename
        size_t startQuote = content.find("\"", binPos + 6);
        size_t endQuote = content.find("\"", startQuote + 1);
        std::string binName = content.substr(startQuote + 1, endQuote - startQuote - 1);
        
        // Load binary file
        std::string binPath = filePath.substr(0, filePath.find_last_of("/\\") + 1) + binName;
        return loadGLTFBinary(binPath, content);
    }
    
    GLTFMeshData loadGLTFBinary(const std::string& binPath, const std::string& jsonContent) {
        GLTFMeshData mesh;
        
        std::cout << "[*] Loading binary: " << binPath << "\n";
        
        // Load binary file
        std::ifstream binFile(binPath, std::ios::binary);
        if (!binFile.is_open()) {
            std::cerr << "[ERROR] Could not open binary file: " << binPath << "\n";
            return createDefaultCube();
        }
        
        std::vector<char> binaryData((std::istreambuf_iterator<char>(binFile)),
                                     std::istreambuf_iterator<char>());
        binFile.close();
        
        std::cout << "[*] Binary size: " << binaryData.size() << " bytes\n";
        
        // Parse accessor 0: positions (VEC3, offset 0, byteStride 12, count 1062)
        // Parse accessor 4: indices (SCALAR, uint32, count 2856)
        
        // From JSON:
        // accessor 0: bufferView 2, offset 0, count 1062, VEC3
        // accessor 4: bufferView 0, offset 0, count 2856, SCALAR (uint32)
        // bufferView 0: byteOffset 0, byteLength 11424
        // bufferView 2: byteOffset 19920, byteStride 12, byteLength 25488
        
        int positionCount = 1062;
        int indexCount = 2856;
        int positionByteOffset = 19920;
        int indicesByteOffset = 0;
        
        std::cout << "[*] Parsing positions from offset " << positionByteOffset << ", count " << positionCount << "\n";
        std::cout << "[*] Parsing indices from offset " << indicesByteOffset << ", count " << indexCount << "\n";
        
        // Extract positions (float3, stride 12)
        if (positionByteOffset + positionCount * 12 <= (int)binaryData.size()) {
            for (int i = 0; i < positionCount; ++i) {
                int offset = positionByteOffset + i * 12;
                float x = *reinterpret_cast<const float*>(binaryData.data() + offset);
                float y = *reinterpret_cast<const float*>(binaryData.data() + offset + 4);
                float z = *reinterpret_cast<const float*>(binaryData.data() + offset + 8);
                mesh.positions.push_back(glm::vec3(x, y, z));
            }
            std::cout << "[OK] Loaded " << mesh.positions.size() << " positions\n";
        } else {
            std::cerr << "[ERROR] Not enough data for positions\n";
            return createDefaultCube();
        }
        
        // Extract indices (uint32)
        if (indicesByteOffset + indexCount * 4 <= (int)binaryData.size()) {
            for (int i = 0; i < indexCount; ++i) {
                int offset = indicesByteOffset + i * 4;
                unsigned int idx = *reinterpret_cast<const unsigned int*>(binaryData.data() + offset);
                mesh.indices.push_back(idx);
            }
            std::cout << "[OK] Loaded " << mesh.indices.size() << " indices\n";
        } else {
            std::cerr << "[ERROR] Not enough data for indices\n";
            return createDefaultCube();
        }
        
        // Extract normals (accessor 1: bufferView 2, offset 12744, count 1062, VEC3)
        int normalByteOffset = 19920 + 12744;
        if (normalByteOffset + positionCount * 12 <= (int)binaryData.size()) {
            for (int i = 0; i < positionCount; ++i) {
                int offset = normalByteOffset + i * 12;
                float nx = *reinterpret_cast<const float*>(binaryData.data() + offset);
                float ny = *reinterpret_cast<const float*>(binaryData.data() + offset + 4);
                float nz = *reinterpret_cast<const float*>(binaryData.data() + offset + 8);
                mesh.normals.push_back(glm::normalize(glm::vec3(nx, ny, nz)));
            }
            std::cout << "[OK] Loaded " << mesh.normals.size() << " normals\n";
        } else {
            std::cout << "[WARNING] Could not load normals, will compute them\n";
            // Compute normals if not available
            mesh.normals.resize(mesh.positions.size(), glm::vec3(0.0f));
            for (size_t i = 0; i < mesh.indices.size(); i += 3) {
                unsigned int i0 = mesh.indices[i];
                unsigned int i1 = mesh.indices[i + 1];
                unsigned int i2 = mesh.indices[i + 2];
                
                if (i0 < mesh.positions.size() && i1 < mesh.positions.size() && i2 < mesh.positions.size()) {
                    glm::vec3 v0 = mesh.positions[i0];
                    glm::vec3 v1 = mesh.positions[i1];
                    glm::vec3 v2 = mesh.positions[i2];
                    
                    glm::vec3 edge1 = v1 - v0;
                    glm::vec3 edge2 = v2 - v0;
                    glm::vec3 normal = glm::cross(edge1, edge2);
                    
                    mesh.normals[i0] += normal;
                    mesh.normals[i1] += normal;
                    mesh.normals[i2] += normal;
                }
            }
            for (auto& n : mesh.normals) {
                if (glm::length(n) > 0.0f) {
                    n = glm::normalize(n);
                }
            }
        }
        
        std::cout << "[OK] GLTF mesh loaded successfully: " << mesh.positions.size() << " vertices, " 
                  << mesh.indices.size() << " indices\n";
        return mesh;
    }
    
    GLTFMeshData createDefaultCube() {
        GLTFMeshData mesh;
        
        mesh.positions = {
            // Front
            {-1.0f, -1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f},
            // Back
            {-1.0f, -1.0f, -1.0f}, {-1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f},
            // Top
            {-1.0f,  1.0f, -1.0f}, {-1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f,  1.0f, -1.0f},
            // Bottom
            {-1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f, -1.0f}, { 1.0f, -1.0f,  1.0f}, {-1.0f, -1.0f,  1.0f},
            // Right
            { 1.0f, -1.0f, -1.0f}, { 1.0f,  1.0f, -1.0f}, { 1.0f,  1.0f,  1.0f}, { 1.0f, -1.0f,  1.0f},
            // Left
            {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f,  1.0f}, {-1.0f,  1.0f,  1.0f}, {-1.0f,  1.0f, -1.0f},
        };
        
        mesh.normals = {
            {0, 0, 1}, {0, 0, 1}, {0, 0, 1}, {0, 0, 1},
            {0, 0, -1}, {0, 0, -1}, {0, 0, -1}, {0, 0, -1},
            {0, 1, 0}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0},
            {0, -1, 0}, {0, -1, 0}, {0, -1, 0}, {0, -1, 0},
            {1, 0, 0}, {1, 0, 0}, {1, 0, 0}, {1, 0, 0},
            {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0}, {-1, 0, 0},
        };
        
        mesh.indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20
        };
        
        return mesh;
    }
};
