#include "pch.h"
#include "FlockingSimulation.h"

FlockingSimulation::FlockingSimulation() = default;
FlockingSimulation::~FlockingSimulation()
{
    ClearAll();
}

void FlockingSimulation::OnInitialize() {}

void FlockingSimulation::OnUpdate(float deltaTime)
{
    for(int i = 0; i < boids.size(); ++i) {
        Boid& boid = boids[i];
        if(boid.status == STATUS::DEAD) {
            continue;
        }
       
        boid.Update(deltaTime, boids);
    }

    int countForDelete = 0;
    for(int i = boids.size() - 1; i >= 0; --i) {
        Boid& boid = boids[i];
        
        if(boid.status == STATUS::DEAD) {
            boids[i] = std::move(boids[boids.size() - 1 - countForDelete]);
            ++countForDelete;
        }
    }

    boids.erase(boids.end() - countForDelete, boids.end());
}

void FlockingSimulation::OnShutdown()
{
    ClearAll();
}

void FlockingSimulation::ClearAll()
{
    for(CollisionBody* body : obstacles) {
        GetPhysicsWorld().destroyCollisionBody(body);
    }
    obstacles.clear();
    boids.clear();
}

void FlockingSimulation::AddObstacle(const float* center, const float* extents)
{
    BoxShape* shape = GetCommonPhysics().createBoxShape(RVector3{extents[0], extents[1], extents[2]});
    // Initial position and orientation of the collision body 
    const RQuaternion orientation = RQuaternion::identity(); 
    const Transform bodyTransform({center[0], center[1], center[2]}, orientation);

    CollisionBody* body = GetPhysicsWorld().createCollisionBody(bodyTransform);
    body->addCollider(shape, Transform::identity());

    obstacles.push_back(body);
}

const float* FlockingSimulation::GetPositionOf(int id) const
{
    return &boids[id].position.x;
}

const vector<Boid>& FlockingSimulation::GetBoids() const
{
    return boids;
}