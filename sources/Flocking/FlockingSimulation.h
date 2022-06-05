#pragma once
#include <map>

#include "Boid.h"

using std::vector;
using std::map;

class FlockingSimulation
{
public:
    FlockingSimulation();
    ~FlockingSimulation();
    
    void OnInitialize();
    void OnUpdate(float deltaTime);
    void OnShutdown();

    void ClearAll();

    template<typename T>
    void Spawn(int boidsCount);

    template<typename T>
    const Boid& Spawn(const float* position, const float* velocity);
    
    void AddObstacle(const float* center, const float* extents);
    const float* GetPositionOf(int id) const;

    const vector<Boid>& GetBoids() const;

#ifndef DEBUG
// protected:
#endif
    template<typename T>
    Boid CreateBoid() const;
    
    vector<Boid> boids;
    vector<CollisionBody*> obstacles;

    const RVector3 minPoint = {-20.f, 0, -20.f};;
    const RVector3 maxPoint = {20.f, 20.0f, 20.f};;
};

template<typename T>
void FlockingSimulation::Spawn(int boidsCount)
{
    boids.reserve(boidsCount);
    
    for(int i = 0; i < boidsCount; ++i) {
        boids.push_back(CreateBoid<T>());
    }
}

template<typename T>
const Boid& FlockingSimulation::Spawn(const float* position, const float* velocity)
{
    Boid boid = CreateBoid<T>();;

    boid.position = RVector3{position[0], position[1], position[2]};
    boid.velocity = RVector3{velocity[0], velocity[1], velocity[2]};

    boids.emplace_back(std::move(boid));
    
    return boids.back();
}

template<typename T>
Boid FlockingSimulation::CreateBoid() const
{
    Boid boid;

    boid.minPoint = &minPoint;
    boid.maxPoint = &maxPoint;
    
    boid.position = RVector3{ GetRandomFloat(minPoint.x, maxPoint.x), GetRandomFloat(minPoint.y, maxPoint.y), GetRandomFloat(minPoint.z, maxPoint.z)};
    boid.velocity = GetRandomVector3().getUnit() * GetRandomFloat(1.f, 10.f);

    boid.behavior = std::make_unique<T>();

    return boid;
}
