#pragma once
#include "Boid.h"

#ifndef BEHAVIOR
#define BEHAVIOR

#include "BehaviorTypes.h"
#include "DefaultBehaviorParams.h"

using std::vector;
struct Boid;

class Behavior
{
public:
    virtual ~Behavior() = default;
    virtual void Perform(float deltaTime, Boid& boid, const vector<Boid>& boids);
    virtual BEHAVIOR_TYPE GetType() const
    {
        return BEHAVIOR_TYPE::DEFAULT;
    }
#ifndef DEBUG
// protected:
#endif
    virtual vector<const Boid*> GetNeighbours(const Boid& boid, const vector<Boid>& boids) const;
    virtual RVector3 GetAlignment(const Boid& boid, const vector<const Boid*>& boids) const;
    virtual RVector3 GetCohesion(const Boid& boid, const vector<const Boid*>& boids) const;
    virtual RVector3 GetSeparation(const Boid& boid, const vector<const Boid*>& boids) const;
    virtual RVector3 GetAvoidance(const Boid& boid) const;
    virtual RVector3 GetUnobstructedDirection(const Boid& boid) const;

    template<typename T>
    vector<const Boid*> GetOfBehavior(const vector<const Boid*>& boids) const;

    float viewDistance = DefaultBehaviorParams::VIEW_DISTANCE;
    float viewAngle = DefaultBehaviorParams::VIEW_ANGLE;
    
    float maxSpeed = DefaultBehaviorParams::MAX_SPEED;
    float minBoidDistance = DefaultBehaviorParams::MIN_BOID_DISTANCE;
    float obstacleAvoidanceDist = DefaultBehaviorParams::OBSTACLE_AVOIDANCE_DIST;
    float obstacleDodgeStrength = DefaultBehaviorParams::OBSTACLE_DODGE_STRENGTH;

    float alignmentWeight = DefaultBehaviorParams::ALIGNMENT_WEIGHT;
    float cohesionWeight = DefaultBehaviorParams::COHESION_WEIGHT;
    float separationWeight = DefaultBehaviorParams::SEPARATION_WEIGHT;
    float avoidanceWeight = DefaultBehaviorParams::AVOIDANCE_WEIGHT;
};

class PreyBehavior : public Behavior
{
public:
    void Perform(float deltaTime, Boid& boid, const vector<Boid>& boids) override;
    BEHAVIOR_TYPE GetType() const override
    {
        return BEHAVIOR_TYPE::PREY;
    }
    
#ifndef DEBUG
// protected:
#endif
    virtual RVector3 GetEscape(const Boid& boid, const vector<const Boid*>& boids) const;

    float escapeWeight = DefaultPreyBehaviorParams::ESCAPE_WEIGHT;
};

class HunterBehavior : public Behavior
{
public:
    HunterBehavior()
    {
        maxSpeed = DefaultHunterBehaviorParams::MAX_SPEED;
    }

    BEHAVIOR_TYPE GetType() const override
    {
        return BEHAVIOR_TYPE::HUNTER;
    }
    
    void Perform(float deltaTime, Boid& boid, const vector<Boid>& boids) override;

    
#ifndef DEBUG
// protected:
#endif
    void ApplyEnergy(float deltaTime, Boid& boid, RVector3& acceleration);
    RVector3 GetHunting(const Boid& boid, const vector<const Boid*>& boids) const;
    bool TryEat(const Boid& boid, const vector<const Boid*>& boids) const;
    bool TryConvert(Boid& boid);

    float acceleratedMaxSpeed = DefaultHunterBehaviorParams::ACCELERATED_MAX_SPEED;
    
    float huntingWeight = DefaultHunterBehaviorParams::HUNTING_WEIGHT;
    float preyVelocityWeight = DefaultHunterBehaviorParams::PREY_VELOCITY_WEIGHT;
    float eatDistance = DefaultHunterBehaviorParams::EAT_DISTANCE;

    float speed = DefaultHunterBehaviorParams::SPEED;
    float acceleration = DefaultHunterBehaviorParams::ACCELERATION;
    float energy = DefaultHunterBehaviorParams::ENERGY;
    float eatEnergy = DefaultHunterBehaviorParams::EAT_ENERGY;
    float timeDominating = DefaultHunterBehaviorParams::TIME_DOMINATING;

    int maxTargetEaten = DefaultHunterBehaviorParams::MAX_TARGETS_EATEN;

    int targetsEaten = 0;
};
#endif