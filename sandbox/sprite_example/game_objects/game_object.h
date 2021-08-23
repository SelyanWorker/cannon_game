#pragma once

#include "sprite/sprite.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>
#include <utility>

namespace cannon_game
{
    class Entity
    {
    public:
        Entity(const glm::vec2& position = { 0, 0 },
               float rotation = 0,
               float scale = 1)
        :   m_position(position),
            m_rotation(rotation),
            m_scale(scale)
        {
            update_model_matrix();
        }

        glm::mat4 getModelMatrix() const
        {
            return m_modelMatrix;
        }

        void setPosition(const glm::vec2& position)
        {
            if (m_position == position)
                return;

            m_position = position;
            update_model_matrix();
        }

        glm::vec2 getPosition() const
        {
            return m_position;
        }

        void setScale(float scale)
        {
            if (m_scale == scale)
                return;

            m_scale = scale;
            update_model_matrix();
        }

        float getScale() const
        {
            return m_scale;
        }

        void setRotation(float rotation)
        {
            if (m_rotation == rotation)
                return;

            m_rotation = rotation;
            update_model_matrix();
        }

        float getRotation() const
        {
            return m_rotation;
        }

    protected:
        void update_model_matrix()
        {
            m_modelMatrix = glm::identity<glm::mat4>();
            m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(m_position, 0));
            m_modelMatrix =
                glm::rotate(m_modelMatrix, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_scale, m_scale, 1));
        }

    protected:
        glm::vec2 m_position;
        float m_rotation;
        float m_scale;

        glm::mat4 m_modelMatrix;
    };

    class GameObject : public Entity
    {
    public:
        GameObject(std::shared_ptr<selyan::Sprite> sprite,
                   const glm::vec2 &position = { 0, 0 },
                   float rotation = 0,
                   float scale = 1)
        :  m_sprite(std::move(sprite))
        {
            m_position = position;
            m_rotation = rotation;
            m_scale = scale;
            update_model_matrix();
            std::cout << glm::to_string(static_cast<Entity*>(this)->getModelMatrix()) << std::endl;
            std::cout << glm::to_string(m_modelMatrix) << std::endl;
        }

        virtual ~GameObject() = default;

        virtual void update(float elapsedTime) {}

        virtual void draw(selyan::Shader *shader)
        {
            assert(shader != nullptr);

            shader->setUniform("modelMatrix", glm::transpose(m_modelMatrix));
            m_sprite->draw(shader);
        }
//
//        void setEntity(const Entity &entity)
//        {
//            m_entity = entity;
//        }
//
//        Entity getEntity() const { return m_entity; }

    protected:
        std::shared_ptr<selyan::Sprite> m_sprite;
        //Entity m_entity;

        glm::mat4 m_modelMatrix;
    };
}
