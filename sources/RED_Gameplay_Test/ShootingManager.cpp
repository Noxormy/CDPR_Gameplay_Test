#include "pch.h"
#include "ShootingManager.h"

void ShootingManager::OnUpdate(float deltaTime, const Camera& camera, FlockingManager& flockingManager, Keyboard& keyboard, Mouse& mouse, GamePad& gamepad)
{
    timeFromLastShootPassed += deltaTime;
    
    const Mouse::State state = mouse.GetState();
    const Vector3 direction = camera.GetViewDirection();
    const Vector3 position = camera.GetPosition() + direction * 3;

    if(timeFromLastShootPassed > TIME_BETWEEN_SHOOTS && state.leftButton) {
        timeFromLastShootPassed = 0;
        flockingManager.SpawnHunter(position, direction);
    }
}
