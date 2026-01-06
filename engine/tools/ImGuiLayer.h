#pragma once

#include <memory>

class ImGuiLayer {
private:
    static ImGuiLayer* instance;

    ImGuiLayer();

public:
    ~ImGuiLayer();
    static ImGuiLayer& getInstance();

    void initialize();
    void shutdown();
    void newFrame();
    void render();

    bool wantsMouse() const;
    bool wantsKeyboard() const;
};
