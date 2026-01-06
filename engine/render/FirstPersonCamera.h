#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class FirstPersonCamera {
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    
    float yaw;
    float pitch;
    float speed;
    float mouseSensitivity;
    
    void updateCameraVectors() {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
        right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        up = glm::normalize(glm::cross(right, front));
    }
    
public:
    FirstPersonCamera(glm::vec3 startPos = glm::vec3(0.0f, 2.0f, 5.0f))
        : position(startPos), up(0.0f, 1.0f, 0.0f),
          yaw(-90.0f), pitch(0.0f), speed(10.0f), mouseSensitivity(0.1f)
    {
        updateCameraVectors();
    }
    
    // Movimento forward/backward (W/S)
    void moveForward(float deltaTime) {
        position += front * speed * deltaTime;
    }
    
    void moveBackward(float deltaTime) {
        position -= front * speed * deltaTime;
    }
    
    // Movimento left/right (A/D)
    void moveLeft(float deltaTime) {
        position -= right * speed * deltaTime;
    }
    
    void moveRight(float deltaTime) {
        position += right * speed * deltaTime;
    }
    
    // Movimento up/down (Space/Ctrl)
    void moveUp(float deltaTime) {
        position += up * speed * deltaTime;
    }
    
    void moveDown(float deltaTime) {
        position -= up * speed * deltaTime;
    }
    
    // Rotação (mouse look)
    void rotate(float xOffset, float yOffset) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;
        
        yaw += xOffset;
        pitch -= yOffset;
        
        // Clamp pitch
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
        
        updateCameraVectors();
    }
    
    // Matrizes para rendering
    glm::mat4 getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }
    
    glm::mat4 getProjectionMatrix(float fov, float aspect, float near, float far) const {
        return glm::perspective(glm::radians(fov), aspect, near, far);
    }
    
    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    glm::vec3 getUp() const { return up; }
    glm::vec3 getRight() const { return right; }
    float getYaw() const { return yaw; }
    float getPitch() const { return pitch; }
    float getSpeed() const { return speed; }
    
    // Setters
    void setPosition(const glm::vec3& pos) { position = pos; }
    void setSpeed(float s) { speed = s; }
};
