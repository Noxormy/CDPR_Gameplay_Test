#pragma once
#include <unordered_map>

#include "IRenderContext.h"
#include "../Flocking/FlockingSimulation.h"


class FlockingManager
{
public:
    void OnInitialize();
    void OnUpdate(float deltaTime);
    void OnUpdate(float deltaTime, DirectX::Keyboard& keyboard, DirectX::Mouse& mouse, DirectX::GamePad& gamepad);
    void OnShutdown();
    void OnRender(cdp_framework::RenderContextPtr& renderContext) const;

    void AddObstacle(const Vector3& position, const Vector3& extents);
    void Spawn(int boidsCount);
    void SpawnHunter(const Vector3& position, const Vector3& direction);

protected:
    FlockingSimulation flockingSimulation;
    std::unordered_map<BEHAVIOR_TYPE, PrimitivePtr> renderObjects;
    std::unordered_map<BEHAVIOR_TYPE, Color> colors;
};
