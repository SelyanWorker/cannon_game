#pragma once

#include <cmath>

#include <glm/exponential.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

namespace cannon_game
{
    struct CollisionRectangle
    {
        glm::vec2 center;
        glm::vec2 size;
    };

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

    bool checkCollision(const CollisionRectangle &first,
                        const CollisionRectangle &second)   // AABB - AABB collision
    {
        // collision x-axis?
        bool collisionX =
            first.center.x + first.size.x / 2.f >= second.center.x - second.size.x / 2.f &&
            second.center.x + second.size.x / 2.f >= first.center.x - first.size.x / 2.f;
        // collision y-axis?
        bool collisionY =
            first.center.y + first.size.y / 2.f >= second.center.y - second.size.y / 2.f &&
            second.center.y + second.size.y / 2.f >= first.center.y - first.size.y / 2.f;
        // collision only if on both axes
        return collisionX && collisionY;
    }

    bool checkCollision(const CollisionCircle &first, const CollisionCircle &second)
    {
        auto distance = glm::length2(first.center - second.center);
        return glm::pow(first.radius + second.radius, 2.f) >= distance;
    }

    bool checkCollision(const CollisionCircle &circle, const CollisionRectangle &rectangle)
    {
        // get center point circle first
        glm::vec2 center(circle.center + circle.radius);
        // get difference vector between both centers
        glm::vec2 difference = center - rectangle.center;
        glm::vec2 aabb_half_extents(rectangle.size.x / 2.0f, rectangle.size.y / 2.0f);
        glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
        // add clamped value to AABB_center and we get the value of box closest to circle
        glm::vec2 closest = rectangle.center + clamped;
        // retrieve vector between center circle and closest point AABB and check if length <=
        // radius
        difference = closest - center;
        std::cout << "difference: " << glm::to_string(difference) << std::endl;
        return glm::length(difference) < circle.radius;
    }

    bool checkCollision(const CollisionRectangle &rectangle, const CollisionCircle &circle)
    {
        return checkCollision(circle, rectangle);
    }
}
