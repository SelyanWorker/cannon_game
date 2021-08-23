#pragma once

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/exponential.hpp>

namespace cannon_game
{
    struct CollisionCircle
    {
        glm::vec2 center;
        float radius;
    };

    bool checkCollision(const CollisionCircle& first, const CollisionCircle& second)
    {
        auto distance = glm::length2(first.center - second.center);
        return glm::pow(first.radius + second.radius, 2.f) >= distance;
    }
}
