#include "../systems/GameplaySystem.h"

GameplaySystem::GameplaySystem(std::shared_ptr<EnvironmentSystem> env)
    : environment(env) {}

void GameplaySystem::update(float deltaTime, const std::vector<EntityPtr>& entities) {
    if (environment) {
        environment->update(deltaTime);
    }
}
