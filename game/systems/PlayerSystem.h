#pragma once

#include "../../engine/scene/System.h"
#include <memory>

class PlayerSystem : public System {
public:
    void update(float deltaTime, const std::vector<EntityPtr>& entities) override;
};

using PlayerSystemPtr = std::shared_ptr<PlayerSystem>;
