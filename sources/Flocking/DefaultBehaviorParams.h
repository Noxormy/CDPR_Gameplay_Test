#pragma once

struct DefaultBehaviorParams
{
    constexpr static float VIEW_DISTANCE = 2.5f;
    constexpr static float VIEW_ANGLE = 100.f;
    
    constexpr static float MAX_SPEED = 5.f;
    constexpr static float MIN_BOID_DISTANCE = 0.5f;
    constexpr static float OBSTACLE_AVOIDANCE_DIST = 1.5f;
    constexpr static float OBSTACLE_DODGE_STRENGTH = 3.f;

    constexpr static float ALIGNMENT_WEIGHT = 0.3f;
    constexpr static float COHESION_WEIGHT = 0.02f;
    constexpr static float SEPARATION_WEIGHT = 0.05f;
    constexpr static float AVOIDANCE_WEIGHT = 0.3f;
};

struct DefaultPreyBehaviorParams : DefaultBehaviorParams
{
    constexpr static float ESCAPE_WEIGHT = ALIGNMENT_WEIGHT / 2;
};

struct DefaultHunterBehaviorParams : DefaultBehaviorParams
{
    constexpr static float HUNTING_WEIGHT = 0.3f;
    constexpr static float PREY_VELOCITY_WEIGHT = 0.4f;
    constexpr static float EAT_DISTANCE = -0.2f;

    constexpr static float ACCELERATED_MAX_SPEED = 15.f;
    constexpr static float MAX_SPEED = ACCELERATED_MAX_SPEED / 2;
    constexpr static float SPEED = 10.f;
    constexpr static float ACCELERATION = 1.f;
    constexpr static float ENERGY = 10.f;
    constexpr static float EAT_ENERGY = 10.f;
    constexpr static float TIME_DOMINATING = 3.f;

    constexpr static int MAX_TARGETS_EATEN = 3;
};
