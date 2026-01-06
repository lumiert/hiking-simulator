#pragma once

#include "../math/MathTypes.h"
#include "Collider.h"
#include <memory>

class RigidBody {
public:
    enum class Type {
        Static,
        Dynamic,
        Kinematic
    };

private:
    Vec3 velocity;
    Vec3 acceleration;
    float mass;
    Type type;
    ColliderPtr collider;

public:
    RigidBody(Type bodyType = Type::Dynamic, float bodyMass = 1.0f);

    void addForce(const Vec3& force);
    void addVelocity(const Vec3& vel);

    Vec3 getVelocity() const { return velocity; }
    Vec3 getAcceleration() const { return acceleration; }
    float getMass() const { return mass; }
    Type getType() const { return type; }

    void setVelocity(const Vec3& vel) { velocity = vel; }
    void setCollider(ColliderPtr col) { collider = col; }
    ColliderPtr getCollider() const { return collider; }
};

using RigidBodyPtr = std::shared_ptr<RigidBody>;
