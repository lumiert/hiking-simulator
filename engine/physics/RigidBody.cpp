#include "../physics/RigidBody.h"

RigidBody::RigidBody(Type bodyType, float bodyMass)
    : velocity(0, 0, 0), acceleration(0, 0, 0),
      mass(bodyMass), type(bodyType), collider(nullptr) {}

void RigidBody::addForce(const Vec3& force) {
    acceleration += force / mass;
}

void RigidBody::addVelocity(const Vec3& vel) {
    velocity += vel;
}
