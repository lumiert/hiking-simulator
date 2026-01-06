#include "../debug/DebugDraw.h"

std::vector<Line> DebugDraw::lines;

void DebugDraw::drawLine(const Vec3& start, const Vec3& end, const Vec3& color) {
    lines.push_back({start, end, color});
}

void DebugDraw::drawBox(const Vec3& center, const Vec3& halfSize, const Vec3& color) {
    Vec3 min = center - halfSize;
    Vec3 max = center + halfSize;

    drawLine(Vec3(min.x, min.y, min.z), Vec3(max.x, min.y, min.z), color);
    drawLine(Vec3(max.x, min.y, min.z), Vec3(max.x, max.y, min.z), color);
    drawLine(Vec3(max.x, max.y, min.z), Vec3(min.x, max.y, min.z), color);
    drawLine(Vec3(min.x, max.y, min.z), Vec3(min.x, min.y, min.z), color);

    drawLine(Vec3(min.x, min.y, max.z), Vec3(max.x, min.y, max.z), color);
    drawLine(Vec3(max.x, min.y, max.z), Vec3(max.x, max.y, max.z), color);
    drawLine(Vec3(max.x, max.y, max.z), Vec3(min.x, max.y, max.z), color);
    drawLine(Vec3(min.x, max.y, max.z), Vec3(min.x, min.y, max.z), color);

    drawLine(Vec3(min.x, min.y, min.z), Vec3(min.x, min.y, max.z), color);
    drawLine(Vec3(max.x, min.y, min.z), Vec3(max.x, min.y, max.z), color);
    drawLine(Vec3(max.x, max.y, min.z), Vec3(max.x, max.y, max.z), color);
    drawLine(Vec3(min.x, max.y, min.z), Vec3(min.x, max.y, max.z), color);
}

void DebugDraw::drawSphere(const Vec3& center, float radius, const Vec3& color) {
    int segments = 16;
    for (int i = 0; i < segments; i++) {
        float angle1 = (i / (float)segments) * PI * 2;
        float angle2 = ((i + 1) / (float)segments) * PI * 2;

        Vec3 p1 = center + Vec3(cos(angle1), 0, sin(angle1)) * radius;
        Vec3 p2 = center + Vec3(cos(angle2), 0, sin(angle2)) * radius;
        drawLine(p1, p2, color);
    }
}

void DebugDraw::drawGrid(float size, float step, const Vec3& color) {
    for (float i = -size; i <= size; i += step) {
        drawLine(Vec3(-size, 0, i), Vec3(size, 0, i), color);
        drawLine(Vec3(i, 0, -size), Vec3(i, 0, size), color);
    }
}

void DebugDraw::clear() {
    lines.clear();
}
