#pragma once

#include "../../engine/scene/Component.h"
#include "../../engine/render/Camera.h"
#include "../../engine/math/Transform.h"

class CameraController : public Component {
private:
    CameraPtr camera;
    Transform transform;
    float sensitivity;
    float distance;

public:
    CameraController(CameraPtr cam);

    void onAttach() override;
    void update(float deltaTime) override;

    void setSensitivity(float sens) { sensitivity = sens; }
    void setDistance(float dist) { distance = dist; }

    CameraPtr getCamera() const { return camera; }
};

using CameraControllerPtr = std::shared_ptr<CameraController>;
