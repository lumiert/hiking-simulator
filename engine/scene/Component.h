#pragma once

#include <memory>

class Entity;
using EntityPtr = std::shared_ptr<Entity>;

class Component {
public:
    virtual ~Component() = default;
    virtual void onAttach() {}
    virtual void onDetach() {}
    virtual void update(float deltaTime) {}
};

using ComponentPtr = std::shared_ptr<Component>;
