#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <typeinfo>

class Component;
class System;

using ComponentPtr = std::shared_ptr<Component>;
using SystemPtr = std::shared_ptr<System>;

class Entity {
private:
    unsigned int id;
    std::string name;
    std::unordered_map<std::string, ComponentPtr> components;
    bool active;

public:
    Entity(unsigned int entityId, const std::string& entityName = "Entity");

    unsigned int getID() const { return id; }
    const std::string& getName() const { return name; }
    void setName(const std::string& n) { name = n; }

    void addComponent(const std::string& name, ComponentPtr component);
    ComponentPtr getComponent(const std::string& name) const;
    bool hasComponent(const std::string& name) const;
    void removeComponent(const std::string& name);

    void setActive(bool isActive) { active = isActive; }
    bool isActive() const { return active; }

    template<typename T>
    std::shared_ptr<T> getComponent() const {
        const std::string& name = typeid(T).name();
        return std::static_pointer_cast<T>(getComponent(name));
    }

    template<typename T>
    void addComponent(std::shared_ptr<T> component) {
        const std::string& name = typeid(T).name();
        addComponent(name, component);
    }
};

using EntityPtr = std::shared_ptr<Entity>;
