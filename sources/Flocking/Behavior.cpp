#include "pch.h"
#include "Behavior.h"

void Behavior::Perform(float deltaTime, Boid& boid, const vector<Boid>& boids)
{
    const vector<const Boid*> neighbours = GetNeighbours(boid, boids);
    
    const RVector3 alignment = GetAlignment(boid, neighbours) * alignmentWeight;
    const RVector3 cohesion = GetCohesion(boid, neighbours) * cohesionWeight;
    const RVector3 separation = GetSeparation(boid, neighbours) * separationWeight;
    const RVector3 avoidance = GetAvoidance(boid) * avoidanceWeight;

    const RVector3 acceleration = alignment + cohesion + separation + avoidance;
    boid.velocity += acceleration.getUnit();
    boid.velocity = clamp(boid.velocity, maxSpeed);
    
    boid.position += boid.velocity * deltaTime;
}


vector<const Boid*> Behavior::GetNeighbours(const Boid& boid, const vector<Boid>& boids) const
{
    static thread_local vector<const Boid*> neighbours;
    neighbours.clear();
    
    for(const Boid& other : boids) {
        if(&other != &boid && GetDistanceBetweenSquare(boid.position, other.position) <= viewDistance && GetAngleBetween(boid.velocity, (other.position - boid.position)) <= viewAngle)
        {
            neighbours.push_back(&other);
        }
    }

    return neighbours;
}

RVector3 Behavior::GetAlignment(const Boid& boid, const vector<const Boid*>& boids) const
{
    if(boids.size() < 1) {
        return RVector3{};
    }
    
    RVector3 alignment;

    for(const Boid* other : boids) {
        alignment += other->velocity;
    }

    return alignment.getUnit();
}

RVector3 Behavior::GetCohesion(const Boid& boid, const vector<const Boid*>& boids) const
{
    if(boids.size() < 1) {
        return RVector3{};
    }
    
    RVector3 cohesion;
    int count = 0;

    for(const Boid* other : boids) {
        cohesion += other->position;
        count++;
    }

    cohesion = (cohesion / static_cast<float>(count)) - boid.position;
    return cohesion.getUnit();
}

RVector3 Behavior::GetSeparation(const Boid& boid, const vector<const Boid*>& boids) const
{
    if(boids.size() < 1) {
        return RVector3{};
    }
    
    RVector3 shift;

    for(const Boid* other : boids) {
        const RVector3 direction = boid.position - other->position;
        
        const float minDist = minBoidDistance + boid.radius;
        const float minDist_2 = minDist * minDist;
        if(direction.lengthSquare() > minDist_2) {
            continue;
        }

        shift += direction.getUnit();
    }

    return shift.getUnit();
}

RVector3 Behavior::GetAvoidance(const Boid& boid) const
{
    RVector3 shift;
    const RVector3& position = boid.position;
    const RVector3& minPoint = *boid.minPoint;
    const RVector3& maxPoint = *boid.maxPoint;

    if(position.x - minPoint.x < obstacleAvoidanceDist || position.y - minPoint.y < obstacleAvoidanceDist || position.z - minPoint.z < obstacleAvoidanceDist) {
        shift += maxPoint - position;
    }
    if(maxPoint.x - position.x < obstacleAvoidanceDist || maxPoint.y - position.y < obstacleAvoidanceDist || maxPoint.z - position.z < obstacleAvoidanceDist) {
        shift += minPoint - position;
    }

    shift += GetUnobstructedDirection(boid);

    return shift.getUnit();
}


RVector3 Behavior::GetUnobstructedDirection(const Boid& boid) const
{
    RVector3 bestDir;

    const RVector3 rayDir = boid.position + (boid.velocity.getUnit() * (obstacleAvoidanceDist + boid.radius));
    const RRay ray = {boid.position, rayDir};

    RaycastCb cb = {[&bestDir, &position = boid.position, &velocity = boid.velocity, &obstacleDodgeStrength = obstacleDodgeStrength](const RaycastInfo& raycastInfo)
    {
        const RVector3 obstacleSteering = velocity.getUnit().cross({0.f, -1.f, 0.f});
        bestDir += (raycastInfo.worldPoint + obstacleSteering * obstacleDodgeStrength) - position;
        return 0.0f;
    }};
    
    GetPhysicsWorld().raycast(ray, &cb);

    return bestDir.getUnit();
}

template<typename T>
vector<const Boid*> Behavior::GetOfBehavior(const vector<const Boid*>& boids) const
{
    vector<const Boid*> boidsOfType;
    for(const Boid* boid : boids) {
        if(dynamic_cast<T*>(boid->behavior.get()) != nullptr) {
            boidsOfType.push_back(boid);
        }
    }

    return boidsOfType;
}

// PreyBehavior

void PreyBehavior::Perform(float deltaTime, Boid& boid, const vector<Boid>& boids)
{
    const vector<const Boid*> neighbours = GetNeighbours(boid, boids);
    const vector<const Boid*> friends = GetOfBehavior<PreyBehavior>(neighbours);
    const vector<const Boid*> enemies = GetOfBehavior<HunterBehavior>(neighbours);

    const RVector3 alignment = GetAlignment(boid, friends) * alignmentWeight;
    const RVector3 cohesion = GetCohesion(boid, friends) * cohesionWeight;
    const RVector3 separation = GetSeparation(boid, neighbours) * separationWeight;
    const RVector3 avoidance = GetAvoidance(boid) * avoidanceWeight;
    const RVector3 escape = GetEscape(boid, enemies) * escapeWeight;

    const RVector3 acceleration = alignment + cohesion + separation + avoidance + escape;
    boid.velocity += acceleration.getUnit();
    boid.velocity = clamp(boid.velocity, maxSpeed);
    
    boid.position += boid.velocity * deltaTime;
}

RVector3 PreyBehavior::GetEscape(const Boid& boid, const vector<const Boid*>& boids) const
{
    RVector3 escape;

    for(const Boid* other : boids) {
        escape += boid.position - other->position;
    }

    return escape.getUnit();
}

// HunterBehavior

void HunterBehavior::Perform(float deltaTime, Boid& boid, const vector<Boid>& boids)
{
    const vector<const Boid*> neighbours = GetNeighbours(boid, boids);
    const vector<const Boid*> friends = GetOfBehavior<HunterBehavior>(neighbours);
    const vector<const Boid*> targets = GetOfBehavior<PreyBehavior>(neighbours);

    const RVector3 separation = GetSeparation(boid, friends) * separationWeight;
    const RVector3 hunting = GetHunting(boid, targets) * huntingWeight;
    const RVector3 avoidance = GetAvoidance(boid) * avoidanceWeight;
    RVector3 velocityAcceleration = (hunting + separation + avoidance);

    ApplyEnergy(deltaTime, boid, velocityAcceleration);
    boid.position += boid.velocity * deltaTime;

    if(TryEat(boid, targets)) {
        ++targetsEaten;
        energy += eatEnergy;
        boid.radius *= 1.1f;
    }

    if(TryConvert(boid)) {
        return;
    }
}

void HunterBehavior::ApplyEnergy(float deltaTime, Boid& boid, RVector3& velocityAcceleration)
{
    if(energy > 0) {
        const float accelerationDt = acceleration * deltaTime;
        energy = std::max(0.f, energy - accelerationDt);
        speed = std::clamp(speed + accelerationDt, 1.f, acceleratedMaxSpeed);

        velocityAcceleration = velocityAcceleration.getUnit() * (1 + accelerationDt);
    } else {
        speed = std::clamp(speed, 1.f, maxSpeed);
    }

    constexpr static int MULTIPLIER = 100;
    boid.velocity += velocityAcceleration / ((timeDominating * MULTIPLIER) + 1);
    boid.velocity = boid.velocity.getUnit() * speed;

    timeDominating = std::max(0.f, timeDominating - deltaTime);
}


RVector3 HunterBehavior::GetHunting(const Boid& boid, const vector<const Boid*>& boids) const
{
    const Boid* bestTarget = nullptr;
    float bestDist_2 = std::numeric_limits<float>::max();

    for(const Boid* other : boids) {
        const float distance = GetDistanceBetweenSquare(boid.position, other->position);
        if(distance < bestDist_2) {
            bestTarget = other;
            bestDist_2 = distance;
        }
    }

    if(bestTarget == nullptr) {
        return RVector3{};
    }
    
    const RVector3 hunting = (bestTarget->position - boid.position).getUnit() + bestTarget->velocity.getUnit() * preyVelocityWeight;
    return hunting.getUnit();
}

bool HunterBehavior::TryEat(const Boid& boid, const vector<const Boid*>& boids) const
{
    for(const Boid* other : boids) {
        if(GetDistanceBetweenSquare(boid.position, other->position) <= boid.radius + eatDistance) {
            other->status = STATUS::DEAD;
            return true;
        }
    }

    return false;
}

bool HunterBehavior::TryConvert(Boid& boid)
{
    if(targetsEaten < maxTargetEaten) {
        return false;
    }
    
    boid.behavior = std::make_unique<PreyBehavior>();
    return true;
}
