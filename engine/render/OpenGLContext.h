#pragma once

#include <GL/glew.h>

class OpenGLContext {
private:
    static OpenGLContext* instance;

    OpenGLContext();

public:
    ~OpenGLContext();
    static OpenGLContext& getInstance();

    bool initialize();
    void makeCurrent();
    void swapBuffers();

    void clear(float r = 0.1f, float g = 0.1f, float b = 0.1f, float a = 1.0f);
    void setViewport(int x, int y, int width, int height);
    void enableDepthTest(bool enable);
    void enableBlending(bool enable);
    void setWireframe(bool enable);
};
