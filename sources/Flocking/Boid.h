#pragma once
#include "pch.h"
#include "Behavior.h"

#ifndef BOID
#define BOID

using std::vector;

class Behavior;
using BehaviorPtr = std::unique_ptr<Behavior>;

enum class STATUS
{
    ALIVE,
    DEAD
};

struct Boid
{
    void Update(float deltaTime, const vector<Boid>&);

    bool operator ==(const Boid&) const;
    bool operator !=(const Boid&) const;

    const RVector3* minPoint;
    const RVector3* maxPoint;
    
    RVector3 position;
    RVector3 velocity;

    float radius = 0.5f;

    BehaviorPtr behavior;
    mutable STATUS status = STATUS::ALIVE;
};

#endif