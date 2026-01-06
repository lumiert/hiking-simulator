#pragma once

#include "Widget.h"
#include <vector>
#include <memory>

class Layout {
public:
    enum class Type {
        Horizontal,
        Vertical,
        Grid
    };

protected:
    std::vector<WidgetPtr> widgets;
    Type type;

public:
    Layout(Type layoutType);
    virtual ~Layout() = default;

    virtual void arrange() = 0;
    void addWidget(WidgetPtr widget);
    void removeWidget(WidgetPtr widget);

    const std::vector<WidgetPtr>& getWidgets() const { return widgets; }
};

using LayoutPtr = std::shared_ptr<Layout>;
