#include "../render/Camera.h"

Camera::Camera()
    : position(0, 0, 5), forward(0, 0, -1), up(0, 1, 0),
      fov(45.0f), aspect(16.0f/9.0f), nearPlane(0.1f), farPlane(1000.0f) {}

Camera::Camera(const Vec3& pos, const Vec3& target, const Vec3& upVec)
    : position(pos), forward(glm::normalize(target - pos)), up(glm::normalize(upVec)),
      fov(45.0f), aspect(16.0f/9.0f), nearPlane(0.1f), farPlane(1000.0f) {}

void Camera::lookAt(const Vec3& target) {
    forward = glm::normalize(target - position);
}

void Camera::setPerspective(float fieldOfView, float aspectRatio, float near, float far) {
    fov = fieldOfView;
    aspect = aspectRatio;
    nearPlane = near;
    farPlane = far;
}

Mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + forward, up);
}

Mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}

Mat4 Camera::getViewProjectionMatrix() const {
    return getProjectionMatrix() * getViewMatrix();
}
