#pragma once

#include "../../engine/scene/Component.h"
#include "../../engine/math/Transform.h"
#include <memory>

class PlayerController : public Component {
private:
    Transform transform;
    float moveSpeed;
    float rotateSpeed;

public:
    PlayerController();

    void onAttach() override;
    void update(float deltaTime) override;

    Transform& getTransform() { return transform; }
    const Transform& getTransform() const { return transform; }

    void setMoveSpeed(float speed) { moveSpeed = speed; }
    void setRotateSpeed(float speed) { rotateSpeed = speed; }
};

using PlayerControllerPtr = std::shared_ptr<PlayerController>;
