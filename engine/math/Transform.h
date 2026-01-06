#pragma once

#include "MathTypes.h"

class Transform {
public:
    Vec3 position;
    Quat rotation;
    Vec3 scale;

    Transform();
    Transform(const Vec3& pos, const Quat& rot = Quat(1, 0, 0, 0), const Vec3& scl = Vec3(1, 1, 1));

    Mat4 getMatrix() const;
    Mat4 getInverseMatrix() const;

    Vec3 getEulerAngles() const;
    void setEulerAngles(const Vec3& euler);

    Vec3 forward() const;
    Vec3 right() const;
    Vec3 up() const;

    void translate(const Vec3& offset, bool worldSpace = false);
    void rotate(const Vec3& axis, float angle, bool worldSpace = false);
    void rotateEuler(const Vec3& euler, bool worldSpace = false);

    Transform operator*(const Transform& other) const;
};
