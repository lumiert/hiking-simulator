#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat4 = glm::mat4;
using Mat3 = glm::mat3;
using Quat = glm::quat;

const float PI = 3.14159265359f;
const float TWO_PI = 6.28318530718f;
const float HALF_PI = 1.57079632679f;

inline float toRadians(float degrees) {
    return degrees * PI / 180.0f;
}

inline float toDegrees(float radians) {
    return radians * 180.0f / PI;
}

inline Vec3 quaternionToEuler(const Quat& q) {
    return glm::eulerAngles(q);
}

inline Quat eulerToQuaternion(const Vec3& euler) {
    return glm::quat(euler);
}
