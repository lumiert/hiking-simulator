#pragma once

#include "../../engine/scene/Component.h"
#include "../../engine/math/Transform.h"

class Entity;

class Entity : public Component {
private:
    unsigned int id;
    std::string name;

public:
    Entity(unsigned int entityId, const std::string& entityName = "Entity");
    
    unsigned int getID() const { return id; }
    const std::string& getName() const { return name; }
};
