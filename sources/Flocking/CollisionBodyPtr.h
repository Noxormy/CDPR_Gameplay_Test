#pragma once

struct CollisionBodyPtr
{
    CollisionBodyPtr(CollisionBody* ptr)
    {
        this->ptr = ptr;
    }

    CollisionBody

private:
    std::unique_ptr<CollisionBody> ptr = nullptr;
};
