#include "../systems/PlayerSystem.h"
#include "../components/PlayerController.h"

void PlayerSystem::update(float deltaTime, const std::vector<EntityPtr>& entities) {
    for (const auto& entity : entities) {
        if (!entity || !entity->isActive()) continue;

        auto playerController = entity->getComponent<PlayerController>();
        if (playerController) {
            playerController->update(deltaTime);
        }
    }
}
