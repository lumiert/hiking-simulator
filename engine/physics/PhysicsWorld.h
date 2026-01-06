#pragma once

#include "RigidBody.h"
#include "../math/MathTypes.h"
#include <vector>
#include <memory>

class PhysicsWorld {
private:
    Vec3 gravity;
    std::vector<RigidBodyPtr> rigidBodies;

public:
    PhysicsWorld();

    void setGravity(const Vec3& g) { gravity = g; }
    Vec3 getGravity() const { return gravity; }

    void addRigidBody(RigidBodyPtr body);
    void removeRigidBody(RigidBodyPtr body);

    void update(float deltaTime);
};
