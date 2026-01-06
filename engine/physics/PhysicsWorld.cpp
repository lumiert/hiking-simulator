#include "../physics/PhysicsWorld.h"
#include <algorithm>

PhysicsWorld::PhysicsWorld() : gravity(0, -9.81f, 0) {}

void PhysicsWorld::addRigidBody(RigidBodyPtr body) {
    rigidBodies.push_back(body);
}

void PhysicsWorld::removeRigidBody(RigidBodyPtr body) {
    auto it = std::find(rigidBodies.begin(), rigidBodies.end(), body);
    if (it != rigidBodies.end()) {
        rigidBodies.erase(it);
    }
}

void PhysicsWorld::update(float deltaTime) {
    for (auto& body : rigidBodies) {
        if (body->getType() == RigidBody::Type::Dynamic) {
            body->addForce(gravity * body->getMass());

            Vec3 velocity = body->getVelocity();
            velocity += body->getAcceleration() * deltaTime;

            body->setVelocity(velocity);
        }
    }
}
