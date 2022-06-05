#include "pch.h"
#include "FlockingManager.h"

void FlockingManager::OnInitialize()
{
    flockingSimulation.OnInitialize();

    renderObjects[BEHAVIOR_TYPE::DEFAULT] = GetEngine().CreateSpherePrimitive(1.f);
    renderObjects[BEHAVIOR_TYPE::PREY] = GetEngine().CreateSpherePrimitive(1.f);
    renderObjects[BEHAVIOR_TYPE::HUNTER] = GetEngine().CreateSpherePrimitive(1.f);
    
    colors[BEHAVIOR_TYPE::DEFAULT] = Colors::Aquamarine;
    colors[BEHAVIOR_TYPE::PREY] = Colors::Yellow;
    colors[BEHAVIOR_TYPE::HUNTER] = Colors::Red;
}

void FlockingManager::OnUpdate(float deltaTime)
{
    flockingSimulation.OnUpdate(deltaTime);
}

void FlockingManager::OnUpdate(float deltaTime, DirectX::Keyboard& keyboard, DirectX::Mouse& mouse, DirectX::GamePad& gamepad)
{
    UNREFERENCED_PARAMETER(keyboard);
    UNREFERENCED_PARAMETER(mouse);
    UNREFERENCED_PARAMETER(gamepad);
    OnUpdate(deltaTime);
}

void FlockingManager::OnRender(cdp_framework::RenderContextPtr& renderContext) const
{
    for(const Boid& boid : flockingSimulation.GetBoids()) {
        renderContext->RenderPrimitive(renderObjects.at(boid.behavior->GetType()), {boid.radius, boid.radius, boid.radius}, Vector3{&boid.position.x}, Vector3::Zero, colors.at(boid.behavior->GetType()));
    }
}

void FlockingManager::OnShutdown()
{
    renderObjects.clear();
    colors.clear();
}

void FlockingManager::AddObstacle(const Vector3& position, const Vector3& extents)
{
    flockingSimulation.AddObstacle(&position.x, &extents.x);
}

void FlockingManager::Spawn(int boidsCount)
{
    flockingSimulation.Spawn<PreyBehavior>(boidsCount);
}

void FlockingManager::SpawnHunter(const Vector3& position, const Vector3& direction)
{
    flockingSimulation.Spawn<HunterBehavior>(&position.x, &direction.x);
}

