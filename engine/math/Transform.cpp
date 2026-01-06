#include "../math/Transform.h"

Transform::Transform() 
    : position(0, 0, 0), rotation(1, 0, 0, 0), scale(1, 1, 1) {}

Transform::Transform(const Vec3& pos, const Quat& rot, const Vec3& scl)
    : position(pos), rotation(rot), scale(scl) {}

Mat4 Transform::getMatrix() const {
    Mat4 posMat = glm::translate(Mat4(1.0f), position);
    Mat4 rotMat = glm::mat4_cast(rotation);
    Mat4 scaleMat = glm::scale(Mat4(1.0f), scale);
    return posMat * rotMat * scaleMat;
}

Mat4 Transform::getInverseMatrix() const {
    return glm::inverse(getMatrix());
}

Vec3 Transform::getEulerAngles() const {
    return glm::eulerAngles(rotation);
}

void Transform::setEulerAngles(const Vec3& euler) {
    rotation = glm::quat(euler);
}

Vec3 Transform::forward() const {
    return glm::normalize(rotation * Vec3(0, 0, -1));
}

Vec3 Transform::right() const {
    return glm::normalize(rotation * Vec3(1, 0, 0));
}

Vec3 Transform::up() const {
    return glm::normalize(rotation * Vec3(0, 1, 0));
}

void Transform::translate(const Vec3& offset, bool worldSpace) {
    if (worldSpace) {
        position += offset;
    } else {
        position += rotation * offset;
    }
}

void Transform::rotate(const Vec3& axis, float angle, bool worldSpace) {
    Quat deltaRot = glm::angleAxis(angle, glm::normalize(axis));
    if (worldSpace) {
        rotation = deltaRot * rotation;
    } else {
        rotation = rotation * deltaRot;
    }
}

void Transform::rotateEuler(const Vec3& euler, bool worldSpace) {
    Quat deltaRot = glm::quat(euler);
    if (worldSpace) {
        rotation = deltaRot * rotation;
    } else {
        rotation = rotation * deltaRot;
    }
}

Transform Transform::operator*(const Transform& other) const {
    return Transform(
        position + rotation * (scale * other.position),
        rotation * other.rotation,
        scale * other.scale
    );
}
