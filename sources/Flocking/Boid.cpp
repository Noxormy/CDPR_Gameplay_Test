#include "pch.h"
#include "Boid.h"

void Boid::Update(float deltaTime, const vector<Boid>& boids)
{
    behavior->Perform(deltaTime, *this, boids);
}


bool Boid::operator ==(const Boid& other) const
{
    if(position != other.position) {
        return false;
    }

    if(velocity != other.velocity) {
        return false;
    }

    if(radius != other.radius) {
        return false;
    }

    if(status != other.status) {
        return false;
    }

    return true;
}

bool Boid::operator !=(const Boid& other) const
{
    return !(*this == other);
}