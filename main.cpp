#include "engine/core/Logger.h"
#include "engine/core/Timer.h"
#include "engine/core/FileSystem.h"
#include "engine/platform/SDLWindow.h"
#include "engine/platform/SDLInput.h"
#include "engine/platform/Time.h"
#include "engine/render/OpenGLContext.h"
#include "engine/render/Renderer.h"
#include "engine/render/Camera.h"
#include "engine/render/Mesh.h"
#include "engine/render/Shader.h"
#include "engine/assets/AssetManager.h"
#include "engine/scene/World.h"
#include "engine/environment/EnvironmentSystem.h"
#include "engine/physics/PhysicsWorld.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Engine {
private:
    std::unique_ptr<SDLWindow> window;
    std::unique_ptr<Timer> timer;
    std::unique_ptr<EnvironmentSystem> environment;
    std::unique_ptr<PhysicsWorld> physics;
    std::shared_ptr<World> world;
    std::shared_ptr<Camera> camera;

    bool running;
    float targetFPS;

public:
    Engine() : running(true), targetFPS(60.0f) {}

    bool initialize(int windowWidth, int windowHeight) {
        LOG_INFO("Initializing Engine...");

        window = std::make_unique<SDLWindow>();
        if (!window->create("Hiking Simulator", windowWidth, windowHeight)) {
            LOG_ERROR("Failed to create window");
            return false;
        }

        if (!OpenGLContext::getInstance().initialize()) {
            LOG_ERROR("Failed to initialize OpenGL");
            return false;
        }

        timer = std::make_unique<Timer>();
        timer->start();

        environment = std::make_unique<EnvironmentSystem>();
        environment->setSunDirection(Vec3(-0.3f, -1.0f, -0.5f));
        environment->setSunColor(Vec3(1.0f, 1.0f, 0.9f));
        environment->setAmbientLight(0.4f);

        physics = std::make_unique<PhysicsWorld>();
        physics->setGravity(Vec3(0, -9.81f, 0));

        world = std::make_shared<World>();

        camera = std::make_shared<Camera>();
        camera->setPosition(Vec3(0, 2, 5));
        camera->lookAt(Vec3(0, 1, 0));
        camera->setPerspective(45.0f, window->getAspectRatio(), 0.1f, 1000.0f);

        Renderer::getInstance().setCamera(camera);

        LOG_INFO("Engine initialized successfully");
        return true;
    }

    void update() {
        timer->tick();
        float deltaTime = timer->getDeltaTime();
        Time::update(deltaTime);

        SDLInput::getInstance().update();

        // Handle input
        if (SDLInput::getInstance().isKeyPressed(SDL_SCANCODE_ESCAPE)) {
            running = false;
        }

        environment->update(deltaTime);
        physics->update(deltaTime);
        world->update(deltaTime);
    }

    void render() {
        OpenGLContext::getInstance().clear(0.5f, 0.7f, 0.9f);
        Renderer::getInstance().render();
        window->swapBuffers();
    }

    void run() {
        while (running) {
            update();
            render();
        }
    }

    void shutdown() {
        LOG_INFO("Shutting down engine...");
        world.reset();
        camera.reset();
        physics.reset();
        environment.reset();
        timer.reset();
        window.reset();
        LOG_INFO("Engine shut down complete");
    }

    bool isRunning() const { return running; }
    void setRunning(bool r) { running = r; }
};

int main(int argc, char* argv[]) {
    Logger::getInstance().init("hiking_simulator.log");
    Logger::getInstance().setMinLevel(LogLevel::Debug);

    LOG_INFO("======================================");
    LOG_INFO("Hiking Simulator - Walkthrough Demo");
    LOG_INFO("======================================");

    Engine engine;
    if (!engine.initialize(1280, 720)) {
        LOG_CRITICAL("Failed to initialize engine");
        return -1;
    }

    engine.run();
    engine.shutdown();

    LOG_INFO("Application terminated successfully");
    return 0;
}
