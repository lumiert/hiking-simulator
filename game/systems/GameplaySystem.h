#pragma once

#include "../../engine/scene/System.h"
#include "../../engine/environment/EnvironmentSystem.h"
#include <memory>

class GameplaySystem : public System {
private:
    std::shared_ptr<EnvironmentSystem> environment;

public:
    GameplaySystem(std::shared_ptr<EnvironmentSystem> env);
    void update(float deltaTime, const std::vector<EntityPtr>& entities) override;
};

using GameplaySystemPtr = std::shared_ptr<GameplaySystem>;
