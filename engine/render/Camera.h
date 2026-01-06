#pragma once

#include "../math/MathTypes.h"
#include <memory>

class Camera {
private:
    Vec3 position;
    Vec3 forward;
    Vec3 up;
    float fov;
    float aspect;
    float nearPlane;
    float farPlane;

public:
    Camera();
    Camera(const Vec3& pos, const Vec3& target, const Vec3& upVec);

    void lookAt(const Vec3& target);
    void setPerspective(float fieldOfView, float aspectRatio, float near, float far);

    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix() const;
    Mat4 getViewProjectionMatrix() const;

    Vec3 getPosition() const { return position; }
    Vec3 getForward() const { return forward; }
    Vec3 getRight() const { return glm::cross(forward, up); }
    Vec3 getUp() const { return up; }

    void setPosition(const Vec3& pos) { position = pos; }
    void setForward(const Vec3& dir) { forward = glm::normalize(dir); }
    void setUp(const Vec3& upVec) { up = glm::normalize(upVec); }
};

using CameraPtr = std::shared_ptr<Camera>;
