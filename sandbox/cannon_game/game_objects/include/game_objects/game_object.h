#pragma once

#include "collision.h"

#include "sprite/sprite.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <utility>

namespace cannon_game
{
    class GameObject
    {
    public:
        GameObject();

        virtual ~GameObject() = default;

        virtual void draw(std::shared_ptr<selyan::Shader> shader) = 0;

        uint32_t getUniqueId() const;

        void setCollision(const CollisionCircle &collision);

        CollisionCircle getCollision() const;

        glm::mat4 getModelMatrix() const;

        void setPosition(const glm::vec2 &position);

        glm::vec2 getPosition() const;

        void setScale(const glm::vec2 &scale);

        glm::vec2 getScale() const;

        void setRotation(float rotation);

        float getRotation() const;

        void die();

        void alive();

        bool isDie() const;

    private:
        void update_model_matrix();

    private:
        uint32_t m_uniqueId;

        glm::vec2 m_position;
        float m_rotation;
        glm::vec2 m_scale;

        glm::mat4 m_modelMatrix;

        CollisionCircle m_collision;

        bool m_dead;

        static uint32_t lastUniqueId;
    };

}
