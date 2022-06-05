#pragma once
#include "Camera.h"
#include "FlockingManager.h"


class ShootingManager
{
public:
    void OnUpdate(float deltaTime, const Camera& camera, FlockingManager& flockingManager, Keyboard& keyboard, Mouse& mouse, GamePad& gamepad);

private:
    constexpr static float TIME_BETWEEN_SHOOTS = 0.3f;
    float timeFromLastShootPassed = TIME_BETWEEN_SHOOTS;
};
