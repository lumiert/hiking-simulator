#pragma once

#include <memory>

class DebugPanels {
private:
    bool showStats;
    bool showProfiler;
    bool showDebugDraw;

public:
    DebugPanels();

    void update();
    void draw();

    void toggleStats() { showStats = !showStats; }
    void toggleProfiler() { showProfiler = !showProfiler; }
    void toggleDebugDraw() { showDebugDraw = !showDebugDraw; }
};

using DebugPanelsPtr = std::shared_ptr<DebugPanels>;
