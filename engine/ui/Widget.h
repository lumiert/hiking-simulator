#pragma once

#include <string>
#include <memory>
#include "../math/MathTypes.h"

class Widget {
protected:
    std::string name;
    Vec2 position;
    Vec2 size;
    bool visible;

public:
    Widget(const std::string& widgetName);
    virtual ~Widget() = default;

    virtual void draw() = 0;
    virtual void update(float deltaTime) {}
    virtual void handleInput() {}

    void setPosition(const Vec2& pos) { position = pos; }
    void setSize(const Vec2& sz) { size = sz; }
    void setVisible(bool vis) { visible = vis; }

    Vec2 getPosition() const { return position; }
    Vec2 getSize() const { return size; }
    bool isVisible() const { return visible; }
};

using WidgetPtr = std::shared_ptr<Widget>;
