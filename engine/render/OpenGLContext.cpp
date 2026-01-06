#include "../render/OpenGLContext.h"
#include "../core/Logger.h"

OpenGLContext* OpenGLContext::instance = nullptr;

OpenGLContext::OpenGLContext() {}

OpenGLContext::~OpenGLContext() {}

OpenGLContext& OpenGLContext::getInstance() {
    if (!instance) {
        instance = new OpenGLContext();
    }
    return *instance;
}

bool OpenGLContext::initialize() {
    LOG_INFO("Initializing OpenGL Context");
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    return true;
}

void OpenGLContext::makeCurrent() {}

void OpenGLContext::swapBuffers() {}

void OpenGLContext::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLContext::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void OpenGLContext::enableDepthTest(bool enable) {
    if (enable) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
}

void OpenGLContext::enableBlending(bool enable) {
    if (enable) glEnable(GL_BLEND);
    else glDisable(GL_BLEND);
    if (enable) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLContext::setWireframe(bool enable) {
    glPolygonMode(GL_FRONT_AND_BACK, enable ? GL_LINE : GL_FILL);
}
