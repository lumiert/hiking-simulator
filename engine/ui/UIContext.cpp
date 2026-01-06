#include "../ui/UIContext.h"

UIContext* UIContext::instance = nullptr;

UIContext::UIContext() : visible(true) {}

UIContext::~UIContext() {}

UIContext& UIContext::getInstance() {
    if (!instance) {
        instance = new UIContext();
    }
    return *instance;
}

void UIContext::addWidget(WidgetPtr widget) {
    if (widget) {
        widgets.push_back(widget);
    }
}

void UIContext::removeWidget(WidgetPtr widget) {
    auto it = std::find(widgets.begin(), widgets.end(), widget);
    if (it != widgets.end()) {
        widgets.erase(it);
    }
}

void UIContext::addLayout(LayoutPtr layout) {
    if (layout) {
        layouts.push_back(layout);
    }
}

void UIContext::removeLayout(LayoutPtr layout) {
    auto it = std::find(layouts.begin(), layouts.end(), layout);
    if (it != layouts.end()) {
        layouts.erase(it);
    }
}

void UIContext::draw() {
    if (!visible) return;

    for (auto& widget : widgets) {
        if (widget && widget->isVisible()) {
            widget->draw();
        }
    }
}

void UIContext::update(float deltaTime) {
    for (auto& widget : widgets) {
        if (widget && widget->isVisible()) {
            widget->update(deltaTime);
        }
    }
}
