#pragma once

#include "collision.h"

namespace cannon_game
{

    class Player   //: public GameObject
    {
    public:
        Player(std::shared_ptr<selyan::Sprite> sprite,
               float collisionRadius = 1,
               const glm::vec2 &position = { 0, 0 },
               float rotation = 0,
               const glm::vec2& scale = { 1, 1 })
        :   m_sprite(sprite),
            m_position(position),
            m_rotation(rotation),
            m_scale(scale),
            m_collisionCircle({ m_position, collisionRadius }),
            m_die(false)
        {
            update_model_matrix();
        }

        void update(float elapsedTime)
        {

        }

        virtual void draw(selyan::Shader *shader)
        {
            assert(shader != nullptr);

            shader->setUniform("modelMatrix", glm::transpose(m_modelMatrix));
            m_sprite->draw(shader);
        }

        glm::vec2 getPosition() const
        {
            return m_position;
        }

        void die()
        {
            std::cout << "I'm die" << std::endl;
            m_die = true;
        }

        void alive()
        {
            m_die = false;
        }

        bool isDie() const
        {
            return m_die;
        }

        CollisionCircle getCollisionCircle() const
        {
            return m_collisionCircle;
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
        std::shared_ptr<selyan::Sprite> m_sprite;

        glm::vec2 m_position;
        float m_rotation;
        glm::vec2 m_scale;

        CollisionCircle m_collisionCircle;

        glm::mat4 m_modelMatrix;

        bool m_die;
    };
}
