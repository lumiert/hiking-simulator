#include "PlayerController.h"
#include "../../engine/platform/SDLInput.h"

PlayerController::PlayerController()
    : transform(Vec3(0, 1, 0)), moveSpeed(5.0f), rotateSpeed(90.0f) {}

void PlayerController::onAttach() {}

void PlayerController::update(float deltaTime) {
    const float speed = moveSpeed * deltaTime;

    if (SDLInput::getInstance().isKeyHeld(SDL_SCANCODE_W)) {
        transform.translate(Vec3(0, 0, -speed), false);
    }
    if (SDLInput::getInstance().isKeyHeld(SDL_SCANCODE_S)) {
        transform.translate(Vec3(0, 0, speed), false);
    }
    if (SDLInput::getInstance().isKeyHeld(SDL_SCANCODE_A)) {
        transform.translate(Vec3(-speed, 0, 0), false);
    }
    if (SDLInput::getInstance().isKeyHeld(SDL_SCANCODE_D)) {
        transform.translate(Vec3(speed, 0, 0), false);
    }

    if (SDLInput::getInstance().isMouseButtonHeld(1)) {
        Vec2 delta = SDLInput::getInstance().getMouseDelta();
        float yaw = -delta.x * rotateSpeed * deltaTime * 0.1f;
        float pitch = -delta.y * rotateSpeed * deltaTime * 0.1f;

        transform.rotateEuler(Vec3(pitch, yaw, 0), false);
    }
}
