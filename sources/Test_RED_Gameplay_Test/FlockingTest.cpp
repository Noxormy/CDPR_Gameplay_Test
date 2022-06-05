#include "pch.h"
#include <reactphysics3d/reactphysics3d.h>

#define DEBUG

#include <Boid.h>
#include <FlockingSimulation.h>

using reactphysics3d::CollisionBody;
using reactphysics3d::Vector3;
using reactphysics3d::PhysicsCommon;
using reactphysics3d::PhysicsWorld;


class FlockingTest : public ::testing::Test
{
public:
    FlockingTest() : boids(flockingSimulation.boids) {}
    
    void SetUp() override
    {
        physicsWorld = physicsCommon.createPhysicsWorld();
        flockingSimulation.OnInitialize();
    }

    void TearDown() override
    {
        physicsCommon.destroyPhysicsWorld(physicsWorld);
    }

    Boid GetBoid(Vector3 position, Vector3 velocity)
    {
        Boid boid = flockingSimulation.CreateBoid<Behavior>();
        boid.position = position;
        boid.velocity = velocity;
        return boid;
    }

    template<typename T>
    const Boid& AddBoid(Vector3 position, Vector3 velocity)
    {
        return flockingSimulation.Spawn<T>(&position.x, &velocity.x);
    }

    vector<Boid>& boids;
    FlockingSimulation flockingSimulation;
    PhysicsCommon physicsCommon;
    PhysicsWorld* physicsWorld;
};

// test cases
////////////////////////////////////////////////

TEST_F( FlockingTest , CheckParams )
{
    vector<Boid> boids;
    boids.push_back(GetBoid(Vector3{1.f, 2.f, 3.f}, Vector3{0.f, 1.f, 0.f}));
    boids.push_back(GetBoid(Vector3{3.f, 2.f, 1.f}, Vector3{1.f, 0.f, 0.f}));
    boids.push_back(GetBoid(Vector3{3.f, 1.f, 2.f}, Vector3{0.f, 0.f, 1.f}));

    Boid& boid_0 = boids[0];
    Boid& boid_1 = boids[1];
    Boid& boid_2 = boids[2];

    ASSERT_EQ(boid_0.position, boid_0.position);
    ASSERT_EQ(boid_0.velocity, boid_0.velocity);
    ASSERT_FLOAT_EQ(boid_0.radius, boid_0.radius);
    
    ASSERT_EQ(boid_1.position, boid_1.position);
    ASSERT_EQ(boid_1.velocity, boid_1.velocity);
    ASSERT_FLOAT_EQ(boid_1.radius, boid_1.radius);

    ASSERT_EQ(boid_2.position, boid_2.position);
    ASSERT_EQ(boid_2.velocity, boid_2.velocity);
    ASSERT_FLOAT_EQ(boid_2.radius, boid_2.radius);
}


TEST_F( FlockingTest, Equality )
{
    const Vector3 position = Vector3{1.f, 2.f, 3.f};
    const Vector3 velocity = Vector3{1.f, 0.f, 0.f};
    
    const Boid boid_0 = {GetBoid(position, velocity)};
    const Boid boid_1 = {GetBoid(position, velocity)};

    ASSERT_EQ(boid_0, boid_1);
}


TEST_F( FlockingTest , CheckAlignment )
{
    const Vector3 position_0 = Vector3{0.f, 0.f, 0.f};
    const Vector3 position_1 = position_0 + Vector3{0.f, 0.f, DefaultBehaviorParams::VIEW_DISTANCE / 2};
    const Vector3 velocity_0 = Vector3{1.f, 0.f, 0.f};
    const Vector3 velocity_1 = Vector3{0.f, 1.f, 0.f};

    AddBoid<Behavior>(position_0, velocity_0);
    AddBoid<Behavior>(position_1, velocity_1);

    vector<const Boid*> neighbours = boids[0].behavior->GetNeighbours(boids[0], boids);
    
    Vector3 alignment = boids[0].behavior->GetAlignment(boids[0], neighbours);
    ASSERT_EQ(alignment, velocity_1.getUnit());
    ASSERT_EQ(alignment.length(), 1.f);

    neighbours = boids[1].behavior->GetNeighbours(boids[1], boids);

    alignment = boids[1].behavior->GetAlignment(boids[1], neighbours);
    ASSERT_EQ(alignment, velocity_0.getUnit());
    ASSERT_EQ(alignment.length(), 1.f);

    const Vector3 position_2 = position_0 + Vector3{0.f, 0.f, -DefaultBehaviorParams::VIEW_DISTANCE / 2};
    const Vector3 velocity_2 = Vector3{0.f, -1.f, 0.f};
    AddBoid<Behavior>(position_2, velocity_2);

    neighbours = boids[0].behavior->GetNeighbours(boids[0], boids);

    alignment = boids[0].behavior->GetAlignment(boids[0], neighbours);
    ASSERT_EQ(alignment, Vector3::zero());

    flockingSimulation.ClearAll();
}

TEST_F( FlockingTest, CheckCohesion )
{
    const Vector3 position_0 = Vector3{0.f, 0.f, 0.f};
    const Vector3 position_1 = position_0 + Vector3{0.f, 0.f, DefaultBehaviorParams::VIEW_DISTANCE / 2};
    const Vector3 velocity_0 = Vector3{1.f, 0.f, 0.f};
    const Vector3 velocity_1 = Vector3{0.f, 1.f, 0.f};

    AddBoid<Behavior>(position_0, velocity_0);
    AddBoid<Behavior>(position_1, velocity_1);

    vector<const Boid*> neighbours = boids[0].behavior->GetNeighbours(boids[0], boids);

    Vector3 cohesion = boids[0].behavior->GetCohesion(boids[0], neighbours);
    ASSERT_EQ(cohesion, (position_1 - position_0).getUnit());
    ASSERT_EQ(cohesion.length(), 1.f);

    neighbours = boids[1].behavior->GetNeighbours(boids[1], boids);
    cohesion = boids[1].behavior->GetCohesion(boids[1], neighbours);
    ASSERT_EQ(cohesion, (position_0 - position_1).getUnit());
    ASSERT_EQ(cohesion.length(), 1.f);

    const Vector3 position_2 = position_0 + Vector3{0.f, 0.f, -DefaultBehaviorParams::VIEW_DISTANCE / 2};
    const Vector3 velocity_2 = Vector3{0.f, -1.f, 0.f};
    AddBoid<Behavior>(position_2, velocity_2);

    neighbours = boids[0].behavior->GetNeighbours(boids[0], boids);
    cohesion = boids[0].behavior->GetCohesion(boids[0], neighbours);
    ASSERT_EQ(cohesion, ((position_1 + position_2) - position_0).getUnit());
    ASSERT_EQ(cohesion.length(), 0.f);

    neighbours = boids[1].behavior->GetNeighbours(boids[1], boids);
    cohesion = boids[1].behavior->GetCohesion(boids[1], neighbours);
    ASSERT_EQ(cohesion, ((position_0 + position_2) - position_1).getUnit());
    ASSERT_EQ(cohesion.length(), 1.f);

    neighbours = boids[2].behavior->GetNeighbours(boids[2], boids);
    cohesion = boids[2].behavior->GetCohesion(boids[2], neighbours);
    ASSERT_EQ(cohesion, ((position_0 + position_1) - position_2).getUnit());
    ASSERT_EQ(cohesion.length(), 1.f);

    flockingSimulation.ClearAll();
}

TEST_F( FlockingTest, CheckCohesionCentered )
{
    AddBoid<Behavior>({1.f, 0.f, 0.f}, {1.f, 0.f, 0.f});
    AddBoid<Behavior>({0.f, 1.f, 0.f}, {1.f, 0.f, 0.f});
    AddBoid<Behavior>({0.f, 0.f, 1.f}, {1.f, 0.f, 0.f});
    AddBoid<Behavior>({-1.f, 0.f, 0.f}, {1.f, 0.f, 0.f});
    AddBoid<Behavior>({0.f, -1.f, 0.f}, {1.f, 0.f, 0.f});
    AddBoid<Behavior>({0.f, 0.f, -1.f}, {1.f, 0.f, 0.f});

    constexpr int CHECK_ID = 6;
    AddBoid<Behavior>({0.f, 0.f, 0.f}, {1.f, 0.f, 0.f});

    Boid& boid = flockingSimulation.boids[CHECK_ID];
    boid.behavior->viewAngle = 360.f;

    vector<const Boid*> neigbours = boid.behavior->GetNeighbours(boid, boids);
    const Vector3 cohesion = boid.behavior->GetCohesion(boid, neigbours);
    ASSERT_EQ(cohesion, Vector3::zero());
    ASSERT_EQ(cohesion.length(), 0.f);

    flockingSimulation.ClearAll();
}

TEST_F( FlockingTest, CheckSeparation )
{
    const Vector3 position_0 = Vector3{0.f, 0.f, 0.f};
    const Vector3 position_1 = position_0 + Vector3{0.f, 0.f, DefaultBehaviorParams::MIN_BOID_DISTANCE / 2};
    const Vector3 velocity_0 = Vector3{1.f, 2.f, 0.f};
    const Vector3 velocity_1 = Vector3{0.f, 1.f, 1.f};

    AddBoid<Behavior>(position_0, velocity_0);
    AddBoid<Behavior>(position_1, velocity_1);

    vector<const Boid*> neighbours = boids[0].behavior->GetNeighbours(boids[0], boids);
    Vector3 separation = boids[0].behavior->GetSeparation(boids[0], neighbours);
    ASSERT_EQ(separation, (position_0 - position_1).getUnit());
    ASSERT_EQ(separation.length(), 1.f);

    const Vector3 position_2 = position_0 + Vector3{0.f, 0.f, -DefaultBehaviorParams::MIN_BOID_DISTANCE / 2};
    const Vector3 velocity_2 = Vector3{0.f, 1.f, 0.f};
    AddBoid<Behavior>(position_2, velocity_2);

    neighbours = boids[0].behavior->GetNeighbours(boids[0], boids);
    separation = boids[0].behavior->GetSeparation(boids[0], neighbours);
    ASSERT_EQ(separation, Vector3::zero());
    ASSERT_EQ(separation.length(), 0.f);

    const Vector3 position_3 = position_0 + Vector3{0.f, 0.f, DefaultBehaviorParams::MIN_BOID_DISTANCE / 2};
    const Vector3 velocity_3 = Vector3{0.f, -1.f, 1.f};
    AddBoid<Behavior>(position_3, velocity_3);

    const Vector3 position_4 = position_0 + Vector3{0.f, 0.0f, -DefaultBehaviorParams::MIN_BOID_DISTANCE / 2};
    const Vector3 velocity_4 = Vector3{1.f, -1.f, 0.f};
    AddBoid<Behavior>(position_4, velocity_4);

    // This boid is further away from boid 0
    const Vector3 position_5 = position_0 + Vector3{0.f, -DefaultBehaviorParams::MIN_BOID_DISTANCE, 0.f};
    const Vector3 velocity_5 = Vector3{1.f, -1.f, 0.f};
    AddBoid<Behavior>(position_5, velocity_5);

    neighbours = boids[0].behavior->GetNeighbours(boids[0], boids);
    separation = boids[0].behavior->GetSeparation(boids[0], neighbours);
    ASSERT_EQ(separation, Vector3::zero());
    ASSERT_EQ(separation.length(), 0.f);

    flockingSimulation.ClearAll();
}

TEST_F( FlockingTest, MaxVelocity )
{
    constexpr float ITERATIONS_COUNT = 100;
    constexpr float ERROR = 0.001f;
    constexpr decimal MAX_SPEED_2 = (DefaultBehaviorParams::MAX_SPEED + ERROR) * (DefaultBehaviorParams::MAX_SPEED + ERROR);
    flockingSimulation.Spawn<Behavior>(100);

    for(int i = 0; i < ITERATIONS_COUNT; ++i) {
        flockingSimulation.OnUpdate(GetRandomFloat());
        for(const Boid& boid : boids) {
            ASSERT_TRUE(boid.velocity.lengthSquare() <= MAX_SPEED_2) << i << " steps iterated, speed: " << boid.velocity.length();
        }
    }
}

TEST_F( FlockingTest, CheckNeighbours )
{
    flockingSimulation.Spawn<Behavior>(100);

    const Boid& boid = boids[33];
    const vector<const Boid*> neighbours = boid.behavior->GetNeighbours(boid, boids);

    for(const Boid* other : neighbours) {
        ASSERT_TRUE(GetDistanceBetween(other->position, boid.position) <= DefaultBehaviorParams::VIEW_DISTANCE);
        ASSERT_TRUE(GetAngleBetween(boid.velocity, other->position - boid.position) <= DefaultBehaviorParams::VIEW_ANGLE);
    }
    
    flockingSimulation.ClearAll();
}

TEST_F( FlockingTest, CheckUpdate)
{
    AddBoid<Behavior>({DefaultBehaviorParams::MIN_BOID_DISTANCE, 0.f, 0.f}, {1.f, 0.f, 0.f});
    AddBoid<Behavior>({0.f, DefaultBehaviorParams::MIN_BOID_DISTANCE, 0.f}, {1.f, 0.f, 0.f});
    AddBoid<Behavior>({0.f, 0.f, DefaultBehaviorParams::MIN_BOID_DISTANCE}, {1.f, 0.f, 0.f});
    AddBoid<Behavior>({-DefaultBehaviorParams::MIN_BOID_DISTANCE, 0.f, 0.f}, {0.f, 1.f, 0.f});
    AddBoid<Behavior>({0.f, -DefaultBehaviorParams::MIN_BOID_DISTANCE, 0.f}, {0.f, 1.f, 0.f});
    AddBoid<Behavior>({0.f, 0.f, -DefaultBehaviorParams::MIN_BOID_DISTANCE}, {0.f, 1.f, 0.f});

    constexpr int CHECK_ID = 6;
    AddBoid<Behavior>({0.f, 0.f, 0.f}, {1.f, 0.f, 0.f});

    const Boid& boid = boids[CHECK_ID];
    vector<const Boid*> neighbours = boid.behavior->GetNeighbours(boids[CHECK_ID], boids);

    const Vector3 alignment = boid.behavior->GetAlignment(boids[CHECK_ID], neighbours) * DefaultBehaviorParams::ALIGNMENT_WEIGHT;
    const Vector3 cohesion = boid.behavior->GetCohesion(boids[CHECK_ID], neighbours) * DefaultBehaviorParams::COHESION_WEIGHT;
    const Vector3 separation = boid.behavior->GetSeparation(boids[CHECK_ID], neighbours) * DefaultBehaviorParams::SEPARATION_WEIGHT;
    const Vector3 avoidance = boid.behavior->GetAvoidance(boids[CHECK_ID]) * DefaultBehaviorParams::AVOIDANCE_WEIGHT;

    const Vector3 positionBeforeUpdate = boids[CHECK_ID].position;
    RVector3 afterUpdateVelocity = alignment + cohesion + separation + avoidance;
    
    afterUpdateVelocity = afterUpdateVelocity.getUnit() + boids[CHECK_ID].velocity;
    afterUpdateVelocity = clamp(afterUpdateVelocity, DefaultBehaviorParams::MAX_SPEED);

    constexpr float DELTA_TIME = 1.f;
    boid.behavior->Perform(DELTA_TIME,boids[CHECK_ID], boids);
     
    const Vector3 velocity = boids[CHECK_ID].velocity;
    const Vector3 position = boids[CHECK_ID].position;

    EXPECT_NEAR(velocity.x, afterUpdateVelocity.x, std::numeric_limits<decimal>::epsilon());
    EXPECT_NEAR(velocity.x, afterUpdateVelocity.x, std::numeric_limits<decimal>::epsilon());
    EXPECT_NEAR(velocity.x, afterUpdateVelocity.x, std::numeric_limits<decimal>::epsilon());
    ASSERT_EQ(position, positionBeforeUpdate + afterUpdateVelocity * DELTA_TIME);

    flockingSimulation.ClearAll();
}

TEST_F( FlockingTest, CheckTypes )
{
    const Boid& boid_0 = AddBoid<Behavior>({0.f, 0.f, 0.f}, {1.f, 0.f, 0.f});
    ASSERT_EQ(dynamic_cast<HunterBehavior*>(boid_0.behavior.get()), nullptr);
    ASSERT_EQ(dynamic_cast<PreyBehavior*>(boid_0.behavior.get()), nullptr);

    const Boid& boid_1 = AddBoid<PreyBehavior>({0.f, 0.f, 0.f}, {1.f, 0.f, 0.f});
    ASSERT_NE(dynamic_cast<PreyBehavior*>(boid_1.behavior.get()), nullptr);

    const Boid& boid_2 = AddBoid<HunterBehavior>({0.f, 0.f, 0.f}, {1.f, 0.f, 0.f});
    ASSERT_NE(dynamic_cast<HunterBehavior*>(boid_2.behavior.get()), nullptr);

    flockingSimulation.ClearAll();
}

// BoidPrey

TEST_F( FlockingTest, CheckEscape )
{
    AddBoid<PreyBehavior>({0.f, 0.f, 0.f}, {1.f, 0.f, 0.f});
    AddBoid<HunterBehavior>({0.f, DefaultBehaviorParams::MIN_BOID_DISTANCE, 0.f}, {1.f, 0.f, 0.f});

    PreyBehavior* behavior = dynamic_cast<PreyBehavior*>(boids[0].behavior.get());
    vector<const Boid*> neighbours = boids[0].behavior->GetNeighbours(boids[0], flockingSimulation.boids);

    RVector3 assumedEscape = RVector3{0.f, -1.f, 0.f};
    const RVector3 escape = behavior->GetEscape(boids[0], neighbours);
    ASSERT_EQ(escape, assumedEscape);

    flockingSimulation.ClearAll();
}

// BoidHunter

TEST_F( FlockingTest, CheckHunting )
{
    AddBoid<HunterBehavior>({0.f, 0.f, 0.f}, {1.f, 0.f, 0.f});
    AddBoid<PreyBehavior>({0.f, DefaultPreyBehaviorParams::MIN_BOID_DISTANCE, 0.f}, {1.f, 0.f, 0.f});
    
    vector<const Boid*> neighbours = boids[0].behavior->GetNeighbours(boids[0], flockingSimulation.boids);

    RVector3 assumedHunting = (RVector3{0.f, 1.f, 0.f} + boids[0].velocity * DefaultHunterBehaviorParams::PREY_VELOCITY_WEIGHT).getUnit();
    const RVector3 hunting = dynamic_cast<HunterBehavior*>(boids[0].behavior.get())->GetHunting(boids[0], neighbours);
    ASSERT_EQ(hunting, assumedHunting);

    flockingSimulation.ClearAll();
}

TEST_F( FlockingTest, Conversion )
{
    AddBoid<HunterBehavior>({0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});
    ASSERT_NE(dynamic_cast<HunterBehavior*>(boids[0].behavior.get()), nullptr);

    HunterBehavior* behavior = dynamic_cast<HunterBehavior*>(boids[0].behavior.get());
    behavior->targetsEaten = behavior->maxTargetEaten;

    flockingSimulation.OnUpdate(1.f);

    ASSERT_NE(dynamic_cast<PreyBehavior*>(boids[0].behavior.get()), nullptr);

    flockingSimulation.ClearAll();
}

TEST_F( FlockingTest, HunterAcceleration )
{
    RVector3 velocity = {0.f, 1.f, 0.f};
    RVector3 position = {
        (flockingSimulation.maxPoint.x + flockingSimulation.minPoint.x) / 2,
        (flockingSimulation.maxPoint.y + flockingSimulation.minPoint.y) / 2,
        (flockingSimulation.maxPoint.z + flockingSimulation.minPoint.z) / 2
    };
    const Boid& boid = AddBoid<HunterBehavior>(position, velocity);
    const HunterBehavior* behavior = dynamic_cast<HunterBehavior*>(boid.behavior.get());

    constexpr float DT = 0.1f;
    const float initialSpeed = behavior->speed;
    const float acceleration = (behavior->acceleration * DT);

    ASSERT_EQ(boid.velocity, velocity);
    
    flockingSimulation.OnUpdate(DT);
    velocity = velocity.getUnit() * (initialSpeed + acceleration);
    position = position + velocity * DT;
    ASSERT_EQ(boid.velocity, velocity);
    ASSERT_EQ(boid.position, position);

    constexpr float ERROR = 0.001f;
    flockingSimulation.OnUpdate(DT);
    velocity = velocity.getUnit() * (initialSpeed + acceleration * 2);
    position = position + velocity * DT;
    ASSERT_NEAR((boid.velocity - velocity).length(), 0.f, ERROR);
    ASSERT_NEAR((boid.position - position).length(), 0.f, ERROR);
}

TEST_F( FlockingTest, Death )
{
    AddBoid<HunterBehavior>({0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});
    AddBoid<PreyBehavior>({0.f, 0.f, 0.f}, {0.f, 1.f, 0.f});

    flockingSimulation.OnUpdate(0.000001f);

    ASSERT_EQ(boids.size(), 1);
    ASSERT_NE(dynamic_cast<HunterBehavior*>(boids[0].behavior.get()), nullptr);
}