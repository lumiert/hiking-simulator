#pragma once

#include "Widget.h"
#include "Layout.h"
#include <vector>
#include <memory>

class UIContext {
private:
    static UIContext* instance;
    std::vector<WidgetPtr> widgets;
    std::vector<LayoutPtr> layouts;
    bool visible;

    UIContext();

public:
    ~UIContext();
    static UIContext& getInstance();

    void addWidget(WidgetPtr widget);
    void removeWidget(WidgetPtr widget);

    void addLayout(LayoutPtr layout);
    void removeLayout(LayoutPtr layout);

    void draw();
    void update(float deltaTime);

    void setVisible(bool vis) { visible = vis; }
    bool isVisible() const { return visible; }
};
