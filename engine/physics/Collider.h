#pragma once

#include "../math/Transform.h"
#include <memory>

class Collider {
public:
    enum class Type {
        Box,
        Sphere,
        Capsule
    };

    virtual ~Collider() = default;
    virtual Type getType() const = 0;
    virtual void setTrigger(bool trigger) = 0;
    virtual bool isTrigger() const = 0;
};

using ColliderPtr = std::shared_ptr<Collider>;
