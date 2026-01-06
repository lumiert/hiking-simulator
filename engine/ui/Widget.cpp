#include "../ui/Widget.h"

Widget::Widget(const std::string& widgetName)
    : name(widgetName), position(0, 0), size(0, 0), visible(true) {}
