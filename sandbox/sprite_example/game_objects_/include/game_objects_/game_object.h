#pragma once

#include "collision.h"

#include "sprite/sprite.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <utility>

namespace cannon_game_
{
    class GameObject
    {
    public:
        GameObject(/*uint32_t uniqueId*/)
        :   m_uniqueId(lastUniqueId++),
            m_position({ 0, 0 }),
            m_rotation(0),
            m_scale({ 1, 1 }),
            m_collision({ m_position }),
            m_dead(false)
        {
            update_model_matrix();
        }

        virtual ~GameObject() = default;

        virtual void draw(selyan::Shader *shader)
        {
            assert(shader != nullptr && m_sprite != nullptr);

            shader->setUniform("modelMatrix", glm::transpose(m_modelMatrix));
            m_sprite->draw(shader);
        }

        uint32_t getUniqueId() const
        {
            return m_uniqueId;
        }

        void setCollision(const CollisionCircle& collision)
        {
            m_collision = collision;
        }

        CollisionCircle getCollision() const
        {
            return m_collision;
        }

        void setSprite(std::shared_ptr<selyan::Sprite> sprite)
        {
            m_sprite = std::move(sprite);
        }

        glm::mat4 getModelMatrix() const { return m_modelMatrix; }

        void setPosition(const glm::vec2 &position)
        {
            if (m_position == position)
                return;

            //m_collision.center += m_position - position;
            m_collision.center = position;
            m_position = position;
            update_model_matrix();
        }

        glm::vec2 getPosition() const { return m_position; }

        void setScale(const glm::vec2& scale)
        {
            if (m_scale == scale)
                return;

            m_scale = scale;
            update_model_matrix();
        }

        glm::vec2 getScale() const { return m_scale; }

        void setRotation(float rotation)
        {
            if (m_rotation == rotation)
                return;

            m_rotation = rotation;
            update_model_matrix();
        }

        float getRotation() const { return m_rotation; }

        void die()
        {
            std::cout << "I'm die" << std::endl;
            m_dead = true;
        }

        void alive()
        {
            m_dead = false;
        }

        bool isDie() const
        {
            return m_dead;
        }

    private:
        void update_model_matrix()
        {
            m_modelMatrix = glm::identity<glm::mat4>();
            m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(m_position, 0));
            m_modelMatrix =
                glm::rotate(m_modelMatrix, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_scale, 1));
        }

    private:
        uint32_t m_uniqueId;

        std::shared_ptr<selyan::Sprite> m_sprite;

        glm::vec2 m_position;
        float m_rotation;
        glm::vec2 m_scale;

        glm::mat4 m_modelMatrix;

        CollisionCircle m_collision;

        bool m_dead;

        static uint32_t lastUniqueId;
    };

    uint32_t GameObject::lastUniqueId = 0;
}
