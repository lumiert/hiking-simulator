#pragma once

#include <memory>
#include <vector>

class Entity;
using EntityPtr = std::shared_ptr<Entity>;

class System {
public:
    virtual ~System() = default;
    virtual void update(float deltaTime, const std::vector<EntityPtr>& entities) = 0;
};

using SystemPtr = std::shared_ptr<System>;
