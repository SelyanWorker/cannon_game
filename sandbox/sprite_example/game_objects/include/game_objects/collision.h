#pragma once

#include <cmath>

#include <glm/exponential.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

namespace cannon_game
{
    struct CollisionCircle
    {
        glm::vec2 center;
        float radius;
    };

    template<typename T>
    struct Collision
    {
        T getCollision() const { return m_collision; }
        void setCollision(T mCollision) { m_collision = mCollision; }

    private:
        T m_collision;
    };

    inline bool checkCollision(const CollisionCircle &first, const CollisionCircle &second)
    {
        auto distance = glm::length2(first.center - second.center);
        return glm::pow(first.radius + second.radius, 2.f) >= distance;
    }

}
