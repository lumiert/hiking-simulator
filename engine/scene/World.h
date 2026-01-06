#pragma once

#include "Entity.h"
#include "System.h"
#include <memory>
#include <vector>
#include <unordered_map>

class World {
private:
    std::vector<EntityPtr> entities;
    std::unordered_map<std::string, SystemPtr> systems;
    unsigned int nextEntityID;

public:
    World();

    EntityPtr createEntity(const std::string& name = "Entity");
    void destroyEntity(unsigned int entityID);
    EntityPtr getEntity(unsigned int entityID) const;

    void addSystem(const std::string& name, SystemPtr system);
    SystemPtr getSystem(const std::string& name) const;

    void update(float deltaTime);

    const std::vector<EntityPtr>& getEntities() const { return entities; }
};

using WorldPtr = std::shared_ptr<World>;
