#include "../ui/Layout.h"

Layout::Layout(Type layoutType) : type(layoutType) {}

void Layout::addWidget(WidgetPtr widget) {
    if (widget) {
        widgets.push_back(widget);
    }
}

void Layout::removeWidget(WidgetPtr widget) {
    auto it = std::find(widgets.begin(), widgets.end(), widget);
    if (it != widgets.end()) {
        widgets.erase(it);
    }
}
