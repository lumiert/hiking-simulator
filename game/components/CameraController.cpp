#include "CameraController.h"
#include "../../engine/platform/SDLInput.h"

CameraController::CameraController(CameraPtr cam)
    : camera(cam), transform(Vec3(0, 2, 5)), sensitivity(1.0f), distance(5.0f) {}

void CameraController::onAttach() {}

void CameraController::update(float deltaTime) {
    if (!camera) return;

    if (SDLInput::getInstance().isMouseButtonHeld(1)) {
        Vec2 delta = SDLInput::getInstance().getMouseDelta();
        transform.rotateEuler(Vec3(-delta.y * sensitivity * deltaTime, -delta.x * sensitivity * deltaTime, 0), true);
    }

    camera->setPosition(transform.position);
    camera->lookAt(transform.position + transform.forward());
}
