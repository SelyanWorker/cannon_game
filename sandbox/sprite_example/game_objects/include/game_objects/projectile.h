#pragma once

#include "game_object.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <utility>

namespace cannon_game
{
    class Projectile : public GameObject
    {
    public:
        Projectile(const selyan::Sprite &sprite,
                   uint32_t parentId,
                   const glm::vec2 &direction,
                   float speed)
          : GameObject(),
            m_sprite(sprite),
            m_parentId(parentId),
            m_direction(direction),
            m_speed(speed)
        {
        }

        void draw(selyan::Shader* shader) override
        {
            assert(shader != nullptr);

            shader->setUniform("modelMatrix", glm::transpose(getModelMatrix()));
            m_sprite.draw(shader);
        }

        void update(float elapsedTime)
        {
            auto position = getPosition();
            position += m_direction * m_speed * elapsedTime;
            setPosition(position);
        }

        uint32_t getParentId() const
        {
            return m_parentId;
        }

        void setParentId(uint32_t parentId)
        {
            m_parentId = parentId;
        }

        const glm::vec2 &getDirection() const { return m_direction; }

        void setDirection(const glm::vec2 &mDirection) { m_direction = mDirection; }

        float getSpeed() const { return m_speed; }

        void setSpeed(float mSpeed) { m_speed = mSpeed; }

    private:
        selyan::Sprite m_sprite;

        uint32_t m_parentId;

        glm::vec2 m_direction;
        float m_speed;
    };
}
