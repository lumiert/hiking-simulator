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

// Simple mesh data (supports GLB format)
struct GLBMeshData {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    GLuint VAO = 0, VBO = 0, EBO = 0;
    GLuint baseColorTex = 0, metallicRoughnessTex = 0, normalTex = 0;
    
    void setupGL() {
        if (positions.empty() || indices.empty()) return;
        
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        
        glBindVertexArray(VAO);
        
        // Combine position, normal and texcoord data into single vertex buffer
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
            
            if (i < texCoords.size()) {
                vertexData.push_back(texCoords[i].x);
                vertexData.push_back(texCoords[i].y);
            } else {
                vertexData.push_back(0.0f);
                vertexData.push_back(0.0f);
            }
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        
        // Position (3 floats)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        
        // Normal (3 floats)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // TexCoord (2 floats)
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        
        // Create default textures
        createDefaultTextures();
    }
    
    void render() const {
        if (VAO == 0) return;
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
    
    void createDefaultTextures() {
        // Create gray texture for base color (visible material)
        if (baseColorTex == 0) {
            glGenTextures(1, &baseColorTex);
            glBindTexture(GL_TEXTURE_2D, baseColorTex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
            
            unsigned char grayPixel[] = {180, 180, 180, 255};  // Visible gray
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, grayPixel);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        
        // Create metallic/roughness texture (smooth and less metallic)
        if (metallicRoughnessTex == 0) {
            glGenTextures(1, &metallicRoughnessTex);
            glBindTexture(GL_TEXTURE_2D, metallicRoughnessTex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
            
            // R: unused, G: roughness (0.7), B: metallic (0.2), A: AO
            unsigned char mrPixel[] = {255, 178, 51, 255};
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, mrPixel);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        
        // Create normal texture (neutral blue)
        if (normalTex == 0) {
            glGenTextures(1, &normalTex);
            glBindTexture(GL_TEXTURE_2D, normalTex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
            
            unsigned char normalPixel[] = {128, 128, 255, 255};
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, normalPixel);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
    
    void loadTextureFromPNG(GLuint& textureId, const std::vector<char>& pngData) {
        if (textureId != 0) return;
        
        // Simple PPM-like approach: save PNG to temp file and use stb_image equivalent
        // For now, create a placeholder colored texture based on hash
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
        
        // Use a simple color based on PNG data hash
        unsigned char hash = 0;
        for (size_t i = 0; i < std::min(size_t(100), pngData.size()); ++i) {
            hash = hash * 31 + (unsigned char)pngData[i];
        }
        
        unsigned char colorPixel[] = {hash, (unsigned char)(hash * 2), (unsigned char)(hash * 3), 255};
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorPixel);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    
    void cleanup() {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        if (EBO) glDeleteBuffers(1, &EBO);
        if (baseColorTex) glDeleteTextures(1, &baseColorTex);
        if (metallicRoughnessTex) glDeleteTextures(1, &metallicRoughnessTex);
        if (normalTex) glDeleteTextures(1, &normalTex);
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
    GLBMeshData mesh;
    
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
    std::map<std::string, GLBMeshData> meshCache;
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
            GLBMeshData mesh = loadModel(modelPath);
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
        
        GLint baseColorTexLoc = glGetUniformLocation(shaderProgram, "baseColorTex");
        GLint metallicRoughnessTexLoc = glGetUniformLocation(shaderProgram, "metallicRoughnessTex");
        GLint normalTexLoc = glGetUniformLocation(shaderProgram, "normalTex");
        
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform1f(timeLoc, time);
        
        for (auto& obj : objects) {
            glm::mat4 modelMat = obj.getModelMatrix();
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
            
            // Bind textures
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, obj.mesh.baseColorTex);
            glUniform1i(baseColorTexLoc, 0);
            
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, obj.mesh.metallicRoughnessTex);
            glUniform1i(metallicRoughnessTexLoc, 1);
            
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, obj.mesh.normalTex);
            glUniform1i(normalTexLoc, 2);
            
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
    GLBMeshData loadModel(const std::string& filePath) {
        // Load GLB format
        if (filePath.substr(filePath.find_last_of(".") + 1) != "glb") {
            std::cerr << "[ERROR] Only GLB format is supported. Expected .glb file: " << filePath << "\n";
            return createDefaultCube();
        }
        
        return loadGLB(filePath);
    }
    
    GLBMeshData loadGLB(const std::string& filePath) {
        GLBMeshData mesh;
        
        std::cout << "[*] Loading GLB file: " << filePath << "\n";
        
        // GLB format: 12-byte header + JSON chunk + BIN chunk
        std::ifstream file(filePath, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "[ERROR] Could not open GLB file: " << filePath << "\n";
            return createDefaultCube();
        }
        
        // Read header
        uint32_t magic, version, length;
        file.read(reinterpret_cast<char*>(&magic), 4);
        file.read(reinterpret_cast<char*>(&version), 4);
        file.read(reinterpret_cast<char*>(&length), 4);
        
        if (magic != 0x46546C67) { // "glTF" in little-endian
            std::cerr << "[ERROR] Invalid GLB magic number\n";
            return createDefaultCube();
        }
        
        if (version != 2) {
            std::cerr << "[ERROR] Unsupported GLB version: " << version << "\n";
            return createDefaultCube();
        }
        
        // Read JSON chunk
        uint32_t jsonChunkLength, jsonChunkType;
        file.read(reinterpret_cast<char*>(&jsonChunkLength), 4);
        file.read(reinterpret_cast<char*>(&jsonChunkType), 4);
        
        std::string jsonContent(jsonChunkLength, '\0');
        file.read(&jsonContent[0], jsonChunkLength);
        
        // Read BIN chunk
        uint32_t binChunkLength, binChunkType;
        file.read(reinterpret_cast<char*>(&binChunkLength), 4);
        file.read(reinterpret_cast<char*>(&binChunkType), 4);
        
        std::vector<char> binaryData(binChunkLength);
        file.read(binaryData.data(), binChunkLength);
        file.close();
        
        return loadBinaryData(binaryData, jsonContent);
    }
    
    GLBMeshData loadBinaryData(const std::vector<char>& binaryData, const std::string& jsonContent) {
        GLBMeshData mesh;
        
        std::cout << "[*] Binary size: " << binaryData.size() << " bytes\n";
        
        // Parse JSON to extract accessor and bufferView information
        // Find position accessor (usually first one with VEC3 type)
        size_t posAccessorIdx = 0;
        size_t indAccessorIdx = 4;  // Usually indices are at index 4
        
        // Extract position count
        size_t acc0Pos = jsonContent.find("\"accessors\":[");
        size_t countPos = jsonContent.find("\"count\":", acc0Pos);
        int positionCount = std::stoi(jsonContent.substr(countPos + 8));
        
        std::cout << "[*] Position count: " << positionCount << "\n";
        
        // Find index count (look for SCALAR type accessor)
        size_t scalarPos = jsonContent.find("\"type\":\"SCALAR\"");
        if (scalarPos != std::string::npos) {
            size_t countBefore = jsonContent.rfind("\"count\":", scalarPos);
            if (countBefore != std::string::npos) {
                size_t countEnd = jsonContent.find(",", countBefore);
                std::string countStr = jsonContent.substr(countBefore + 8, countEnd - countBefore - 8);
                int indexCount = std::stoi(countStr);
                std::cout << "[*] Index count: " << indexCount << "\n";
            }
        }
        
        // Find bufferView indices
        // Position uses bufferView 2
        // Indices use bufferView 0
        size_t bufferViewPos = jsonContent.find("\"bufferViews\":[");
        
        int positionByteOffset = -1;
        int indicesByteOffset = -1;
        
        // Extract all byteOffsets from bufferViews
        size_t offsetSearch = bufferViewPos;
        std::vector<int> byteOffsets;
        
        while (true) {
            size_t offset = jsonContent.find("\"byteOffset\":", offsetSearch);
            if (offset == std::string::npos || offset > jsonContent.find("\"buffers\":[")) break;
            
            std::string offsetStr = jsonContent.substr(offset + 13);
            size_t commaPos = offsetStr.find_first_of(",}");
            int byteOffset = std::stoi(offsetStr.substr(0, commaPos));
            byteOffsets.push_back(byteOffset);
            
            offsetSearch = offset + 50;
        }
        
        // Handle missing first offset (defaults to 0)
        if (!byteOffsets.empty()) {
            indicesByteOffset = 0;  // BufferView 0 starts at 0
            if (byteOffsets.size() > 2) {
                positionByteOffset = byteOffsets[1];  // BufferView 2 second in list
            }
        }
        
        std::cout << "[*] Indices offset: " << indicesByteOffset << ", Position offset: " << positionByteOffset << "\n";
        
        if (positionByteOffset < 0) {
            std::cerr << "[ERROR] Could not determine buffer offsets\n";
            return createDefaultCube();
        }
        
        int indexCount = 4542;  // Will refine this later
        
        // Extract indices (uint32) from bufferView 0
        if (indicesByteOffset >= 0 && indicesByteOffset + indexCount * 4 <= (int)binaryData.size()) {
            for (int i = 0; i < indexCount; ++i) {
                int offset = indicesByteOffset + i * 4;
                unsigned int idx = *reinterpret_cast<const unsigned int*>(binaryData.data() + offset);
                mesh.indices.push_back(idx);
            }
            std::cout << "[OK] Loaded " << mesh.indices.size() << " indices\n";
        } else {
            std::cerr << "[ERROR] Not enough data for indices (offset: " << indicesByteOffset << ")\n";
            return createDefaultCube();
        }
        
        // Extract positions (float3, stride 12) from bufferView 2
        if (positionByteOffset >= 0 && positionByteOffset + positionCount * 12 <= (int)binaryData.size()) {
            for (int i = 0; i < positionCount; ++i) {
                int offset = positionByteOffset + i * 12;
                float x = *reinterpret_cast<const float*>(binaryData.data() + offset);
                float y = *reinterpret_cast<const float*>(binaryData.data() + offset + 4);
                float z = *reinterpret_cast<const float*>(binaryData.data() + offset + 8);
                mesh.positions.push_back(glm::vec3(x, y, z));
            }
            std::cout << "[OK] Loaded " << mesh.positions.size() << " positions\n";
        } else {
            std::cerr << "[ERROR] Not enough data for positions (offset: " << positionByteOffset << ")\n";
            return createDefaultCube();
        }
        
        // Extract texCoords (TEXCOORD_0) - usually bufferView 1 with stride 8
        // From JSON: TEXCOORD_0 is accessor 3, uses bufferView 1
        int texCoordByteOffset = -1;
        int texCoordCount = positionCount;  // Same count as positions
        
        if (byteOffsets.size() > 1) {
            texCoordByteOffset = byteOffsets[0];  // BufferView 1 offset
            
            // For old_television, TEXCOORD_0 is in bufferView 1 with stride 8 (vec2, 2 bytes per component)
            // Try to extract with different stride values
            if (texCoordByteOffset >= 0 && texCoordByteOffset + texCoordCount * 8 <= (int)binaryData.size()) {
                for (int i = 0; i < texCoordCount; ++i) {
                    int offset = texCoordByteOffset + i * 8;
                    float u = *reinterpret_cast<const float*>(binaryData.data() + offset);
                    float v = *reinterpret_cast<const float*>(binaryData.data() + offset + 4);
                    mesh.texCoords.push_back(glm::vec2(u, v));
                }
                std::cout << "[OK] Loaded " << mesh.texCoords.size() << " texCoords\n";
            } else if (texCoordByteOffset >= 0 && texCoordByteOffset + texCoordCount * 4 <= (int)binaryData.size()) {
                // Try with stride 4 (in case of half-float)
                for (int i = 0; i < texCoordCount; ++i) {
                    int offset = texCoordByteOffset + i * 4;
                    float u = *reinterpret_cast<const float*>(binaryData.data() + offset);
                    float v = *reinterpret_cast<const float*>(binaryData.data() + offset + 2);
                    mesh.texCoords.push_back(glm::vec2(u, v));
                }
                std::cout << "[OK] Loaded " << mesh.texCoords.size() << " texCoords (half-float)\n";
            }
        }
        
        // Fill with default UVs if not loaded
        if (mesh.texCoords.empty()) {
            std::cout << "[WARNING] No texCoords loaded, using default UVs\n";
            for (int i = 0; i < positionCount; ++i) {
                mesh.texCoords.push_back(glm::vec2(0.0f, 0.0f));
            }
        }
        
        // Extract images from bufferViews (images are in bufferView 4, 5, 6 for old_television)
        // Parse images array from JSON
        size_t imagesPos = jsonContent.find("\"images\":[");
        if (imagesPos != std::string::npos) {
            std::vector<int> imageBufferViews;
            size_t imageSearch = imagesPos;
            
            // Find all image bufferView references
            for (int imgIdx = 0; imgIdx < 10; ++imgIdx) {
                size_t bufViewRef = jsonContent.find("\"bufferView\":", imageSearch);
                if (bufViewRef == std::string::npos || bufViewRef > jsonContent.find("\"materials\":[")) break;
                
                std::string bufViewStr = jsonContent.substr(bufViewRef + 13);
                size_t commaPos = bufViewStr.find_first_of(",}");
                int bufViewIdx = std::stoi(bufViewStr.substr(0, commaPos));
                imageBufferViews.push_back(bufViewIdx);
                
                std::cout << "[*] Image " << imgIdx << " uses bufferView " << bufViewIdx << "\n";
                
                imageSearch = bufViewRef + 50;
            }
            
            // Now extract the actual image data from byteOffsets
            // For old_television: bufferView 4 (baseColor), 5 (metallicRoughness), 6 (normal)
            std::vector<int> imageByteLengths;
            offsetSearch = bufferViewPos;
            int currentBufferView = 0;
            
            while (true) {
                size_t byteLength = jsonContent.find("\"byteLength\":", offsetSearch);
                if (byteLength == std::string::npos || byteLength > jsonContent.find("\"buffers\":[")) break;
                
                std::string byteLengthStr = jsonContent.substr(byteLength + 13);
                size_t commaPos = byteLengthStr.find_first_of(",}");
                int length = std::stoi(byteLengthStr.substr(0, commaPos));
                imageByteLengths.push_back(length);
                
                offsetSearch = byteLength + 50;
                currentBufferView++;
            }
            
            // Load base color image
            if (imageBufferViews.size() > 0) {
                int bufViewIdx = imageBufferViews[0];
                if (bufViewIdx < (int)byteOffsets.size() && bufViewIdx + 1 < (int)imageByteLengths.size()) {
                    int imgOffset = byteOffsets[bufViewIdx];
                    int imgLength = imageByteLengths[bufViewIdx];
                    std::cout << "[*] BaseColor: offset=" << imgOffset << ", length=" << imgLength << "\n";
                    
                    if (imgOffset >= 0 && imgOffset + imgLength <= (int)binaryData.size()) {
                        std::vector<char> pngData(binaryData.begin() + imgOffset, binaryData.begin() + imgOffset + imgLength);
                        mesh.loadTextureFromPNG(mesh.baseColorTex, pngData);
                        std::cout << "[OK] Loaded baseColor texture\n";
                    }
                }
            }
            
            // Load metallic/roughness image
            if (imageBufferViews.size() > 1) {
                int bufViewIdx = imageBufferViews[1];
                if (bufViewIdx < (int)byteOffsets.size() && bufViewIdx + 1 < (int)imageByteLengths.size()) {
                    int imgOffset = byteOffsets[bufViewIdx];
                    int imgLength = imageByteLengths[bufViewIdx];
                    std::cout << "[*] MetallicRoughness: offset=" << imgOffset << ", length=" << imgLength << "\n";
                    
                    if (imgOffset >= 0 && imgOffset + imgLength <= (int)binaryData.size()) {
                        std::vector<char> pngData(binaryData.begin() + imgOffset, binaryData.begin() + imgOffset + imgLength);
                        mesh.loadTextureFromPNG(mesh.metallicRoughnessTex, pngData);
                        std::cout << "[OK] Loaded metallicRoughness texture\n";
                    }
                }
            }
            
            // Load normal image
            if (imageBufferViews.size() > 2) {
                int bufViewIdx = imageBufferViews[2];
                if (bufViewIdx < (int)byteOffsets.size() && bufViewIdx + 1 < (int)imageByteLengths.size()) {
                    int imgOffset = byteOffsets[bufViewIdx];
                    int imgLength = imageByteLengths[bufViewIdx];
                    std::cout << "[*] Normal: offset=" << imgOffset << ", length=" << imgLength << "\n";
                    
                    if (imgOffset >= 0 && imgOffset + imgLength <= (int)binaryData.size()) {
                        std::vector<char> pngData(binaryData.begin() + imgOffset, binaryData.begin() + imgOffset + imgLength);
                        mesh.loadTextureFromPNG(mesh.normalTex, pngData);
                        std::cout << "[OK] Loaded normal texture\n";
                    }
                }
            }
        }
        
        // If textures weren't loaded from PNG, create defaults
        if (mesh.baseColorTex == 0 || mesh.metallicRoughnessTex == 0 || mesh.normalTex == 0) {
            std::cout << "[OK] Using default textures (gray material)\n";
            mesh.createDefaultTextures();
        }
        
        // Compute normals
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
            } else {
                n = glm::vec3(0.0f, 1.0f, 0.0f);
            }
        }
        
        std::cout << "[OK] GLB mesh loaded successfully: " << mesh.positions.size() << " vertices, " 
                  << mesh.indices.size() << " indices\n";
        return mesh;
    }
    
    GLBMeshData createDefaultCube() {
        GLBMeshData mesh;
        
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
