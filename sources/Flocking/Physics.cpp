#include "pch.h"
#include "reactphysics3d/reactphysics3d.h"

using namespace reactphysics3d;

#pragma warning(disable : 4061)

struct Physics
{
    PhysicsCommon physicsCommon;
    PhysicsWorld* physicsWorld;
    
    Physics()
    {
        physicsWorld = physicsCommon.createPhysicsWorld();
    }

    ~Physics()
    {
        physicsCommon.destroyPhysicsWorld(physicsWorld);
    }
    
};

namespace
{
    Physics physics;
}

extern PhysicsCommon& GetCommonPhysics() noexcept
{
    return physics.physicsCommon;
}

extern PhysicsWorld& GetPhysicsWorld() noexcept
{
    assert ( physics.physicsWorld );
    return *physics.physicsWorld;
}