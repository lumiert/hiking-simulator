#include "../scene/Entity.h"

Entity::Entity(unsigned int entityId, const std::string& entityName)
    : id(entityId), name(entityName), active(true) {}

void Entity::addComponent(const std::string& name, ComponentPtr component) {
    components[name] = component;
    if (component) {
        component->onAttach();
    }
}

ComponentPtr Entity::getComponent(const std::string& name) const {
    auto it = components.find(name);
    return it != components.end() ? it->second : nullptr;
}

bool Entity::hasComponent(const std::string& name) const {
    return components.find(name) != components.end();
}

void Entity::removeComponent(const std::string& name) {
    auto it = components.find(name);
    if (it != components.end()) {
        if (it->second) {
            it->second->onDetach();
        }
        components.erase(it);
    }
}
