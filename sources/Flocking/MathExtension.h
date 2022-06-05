#pragma once
#include <corecrt_math_defines.h>
#include <random>

//------------------------------------------------------------------------------
// Primitive Types
namespace reactphysics3d
{
    inline float GetRandomFloat(float min = 0, float max = 1)
    {
        static std::random_device rd;
        static std::default_random_engine engine(rd());
        std::uniform_real_distribution<> uni(min, max);
        return uni(engine);
    }

    inline Vector3 GetRandomVector3()
    {
        return {GetRandomFloat(0.f, 100.f), GetRandomFloat(0.f, 100.f), GetRandomFloat(0.f, 100.f)};
    }

    inline float GetAngleBetween(Vector3 a, Vector3 b)
    {
        a.normalize();
        b.normalize();
        return acos(a.dot(b)) * 180 / M_PI;
    }

    inline float GetDistanceBetween(const Vector3& a, const Vector3& b)
    {
        const Vector3 dir = a - b;
        return dir.length();
    }

    inline float GetDistanceBetweenSquare(const Vector3& a, const Vector3& b)
    {
        const Vector3 dir = a - b;
        return dir.lengthSquare();
    }

    inline float AnglesToRadians(float angles)
    {
        return angles / 180 * M_PI;
    }

    class RaycastCb : public RaycastCallback
    {
        std::function<decimal(const RaycastInfo&)> cb;

    public:
        RaycastCb(std::function<decimal(const RaycastInfo&)> cb)
        {
            this->cb = cb;
        }

    protected:
        decimal notifyRaycastHit(const RaycastInfo& raycastInfo) override
        {
            return cb(raycastInfo);
        }
    };

    template<typename T, typename SRC>
    inline T polymorphic_cast(SRC x)
    {
        #ifdef DEBUG
        assert(dynamic_cast<T>(x) == x); // detect logic error
        #endif

        return static_cast<T>(x);
    }
}

