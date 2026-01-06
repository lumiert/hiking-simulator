#include "../scene/World.h"
#include <algorithm>

World::World() : nextEntityID(0) {}

EntityPtr World::createEntity(const std::string& name) {
    auto entity = std::make_shared<Entity>(nextEntityID++, name);
    entities.push_back(entity);
    return entity;
}

void World::destroyEntity(unsigned int entityID) {
    auto it = std::find_if(entities.begin(), entities.end(),
        [entityID](const EntityPtr& e) { return e->getID() == entityID; });

    if (it != entities.end()) {
        entities.erase(it);
    }
}

EntityPtr World::getEntity(unsigned int entityID) const {
    for (const auto& entity : entities) {
        if (entity->getID() == entityID) {
            return entity;
        }
    }
    return nullptr;
}

void World::addSystem(const std::string& name, SystemPtr system) {
    systems[name] = system;
}

SystemPtr World::getSystem(const std::string& name) const {
    auto it = systems.find(name);
    return it != systems.end() ? it->second : nullptr;
}

void World::update(float deltaTime) {
    for (auto& entity : entities) {
        if (entity->isActive()) {
            // Update components
        }
    }

    for (auto& system : systems) {
        if (system.second) {
            system.second->update(deltaTime, entities);
        }
    }
}
