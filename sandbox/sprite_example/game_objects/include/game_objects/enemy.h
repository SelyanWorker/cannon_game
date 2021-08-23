#pragma once

#include "sprite/sprite.h"
#include "collision.h"

#include <memory>
#include <glm/gtc/matrix_transform.hpp>
#include <utility>

namespace cannon_game
{
//    namespace utility
//    {
//        glm::vec2 calc_position_by_time(float distanceToPlayer, float prevAngle, float elapsedTime)
//        {
//            float phi = glm::radians(prevAngle) - elapsedTime;
//            return glm::vec2(distanceToPlayer * glm::cos(phi),
//                             distanceToPlayer * glm::sin(phi));
//        }
//    }

    class Enemy //: public GameObject
    {
    public:
        using ShootingFunctionType =
            std::function<void(const glm::vec2 &position)>;

    public:
        Enemy(std::shared_ptr<selyan::Sprite> sprite,
              const ShootingFunctionType& shootingFunction,
              float reloadTime,
              float collisionRadius = 1,
              const glm::vec2& center = { 0, 0 },
              float distanceToPlayer = 1,
              float spawnAngle = 0,
              float angularVelocity = 1,
              float scale = 1)
        :   m_sprite(std::move(sprite)),
            m_center(center),
            m_distanceToPlayer(distanceToPlayer),
            m_angularVelocity(angularVelocity),
            m_lastAngle(spawnAngle),
            m_scale(scale),
            m_shootingFun(shootingFunction),
            m_reloadTime(reloadTime),
            m_lastShotTime(0),
            m_collisionCircle({ { 0, 0 }, collisionRadius }),
            m_die(false)
        {
            update_position_and_rotation(0);
        }

        void setShootingFunction(const ShootingFunctionType& shootingFun)
        {
            m_shootingFun = shootingFun;
        }

        void update(float elapsedTime)
        {
            update_position_and_rotation(elapsedTime);

            m_lastShotTime += elapsedTime;
            if (m_lastShotTime >= m_reloadTime)
            {
                m_shootingFun(m_position);
                m_lastShotTime = 0;
            }
        }

        virtual void draw(selyan::Shader *shader)
        {
            assert(shader != nullptr);

            shader->setUniform("modelMatrix", glm::transpose(m_modelMatrix));
            m_sprite->draw(shader);
        }

        void die()
        {
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
        void update_position_and_rotation(float elapsedTime)
        {
            float phi = glm::radians(m_lastAngle) - elapsedTime;

            m_lastAngle += m_angularVelocity * elapsedTime;
            if (m_lastAngle >= 360.)
                m_lastAngle -= 360.;
            auto newPosition = m_center + glm::vec2(m_distanceToPlayer * glm::cos(phi),
                                         m_distanceToPlayer * glm::sin(phi));

            m_position = newPosition;
            m_rotation = m_lastAngle;

            m_collisionCircle.center = m_position;

            update_model_matrix();
        }

        void update_model_matrix()
        {
            m_modelMatrix = glm::identity<glm::mat4>();
            m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(m_position, 0));
            m_modelMatrix =
                glm::rotate(m_modelMatrix, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_scale, m_scale, 1));
        }

    private:
        std::shared_ptr<selyan::Sprite> m_sprite;

        glm::vec2 m_center;
        float m_distanceToPlayer;
        float m_angularVelocity;
        float m_lastAngle;

        glm::vec2 m_position;
        float m_rotation;
        float m_scale;

        glm::mat4 m_modelMatrix;

        ShootingFunctionType m_shootingFun;
        float m_reloadTime;
        float m_lastShotTime;

        CollisionCircle m_collisionCircle;

        bool m_die;
    };
}
