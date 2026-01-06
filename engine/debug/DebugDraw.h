#pragma once

#include "../math/MathTypes.h"
#include <vector>

struct Line {
    Vec3 start, end;
    Vec3 color;
};

struct DebugDraw {
    static std::vector<Line> lines;

    static void drawLine(const Vec3& start, const Vec3& end, const Vec3& color = Vec3(1, 1, 1));
    static void drawBox(const Vec3& center, const Vec3& halfSize, const Vec3& color = Vec3(1, 1, 1));
    static void drawSphere(const Vec3& center, float radius, const Vec3& color = Vec3(1, 1, 1));
    static void drawGrid(float size, float step, const Vec3& color = Vec3(0.5f, 0.5f, 0.5f));
    static void clear();
};
