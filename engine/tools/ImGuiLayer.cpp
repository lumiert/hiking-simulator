#include "../tools/ImGuiLayer.h"

ImGuiLayer* ImGuiLayer::instance = nullptr;

ImGuiLayer::ImGuiLayer() {}

ImGuiLayer::~ImGuiLayer() {}

ImGuiLayer& ImGuiLayer::getInstance() {
    if (!instance) {
        instance = new ImGuiLayer();
    }
    return *instance;
}

void ImGuiLayer::initialize() {}

void ImGuiLayer::shutdown() {}

void ImGuiLayer::newFrame() {}

void ImGuiLayer::render() {}

bool ImGuiLayer::wantsMouse() const { return false; }

bool ImGuiLayer::wantsKeyboard() const { return false; }
