#pragma once

#include <reactphysics3d/reactphysics3d.h>

#include "MathExtension.h"

extern reactphysics3d::PhysicsCommon& GetCommonPhysics() noexcept;
extern reactphysics3d::PhysicsWorld& GetPhysicsWorld() noexcept;

using reactphysics3d::CollisionBody;
using reactphysics3d::RaycastInfo;
using reactphysics3d::RaycastCb;
using reactphysics3d::decimal;
using reactphysics3d::BoxShape;
using reactphysics3d::Transform;

using reactphysics3d::GetRandomFloat;
using reactphysics3d::GetRandomVector3;
using reactphysics3d::GetAngleBetween;
using reactphysics3d::GetDistanceBetween;
using reactphysics3d::AnglesToRadians;

using reactphysics3d::polymorphic_cast;

using RVector3 = reactphysics3d::Vector3;
using RRay = reactphysics3d::Ray;
using RQuaternion = reactphysics3d::Quaternion;
