#pragma once

#include "collision.h"

#include "sprite/sprite.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <utility>

namespace cannon_game
{
    struct ProjectileMovement
    {
        glm::vec2 direction;
        float speed;
    };

    struct ProjectileEntity
    {
        glm::vec2 position = { 0, 0 };
        float rotation = 0;
        float scale = 1;
    };

    class Projectile
    {
    public:
        Projectile(std::shared_ptr<selyan::Sprite> sprite,
                   float collisionRadius,
                   const ProjectileMovement &projectileMovement,
                   const ProjectileEntity &projectileEntity)
          : m_sprite(std::move(sprite)),
            m_projectileMovement(projectileMovement),
            m_projectileEntity(projectileEntity),
            m_collisionCircle({ projectileEntity.position, collisionRadius }),
            m_die(false)
        {
            update_model_matrix();
        }

//        template<typename ...ArgcT>
//        void update(ArgcT... argc)
//        {
//            update_(argc...);
//        }

        void update(float elapsedTime)
        {
            update_position(elapsedTime);
        }

        virtual void draw(selyan::Shader *shader)
        {
            assert(shader != nullptr);

            shader->setUniform("modelMatrix", glm::transpose(m_modelMatrix));
            m_sprite->draw(shader);
        }

        void setProjectileMovement(const ProjectileMovement& projectileMovement)
        {
            m_projectileMovement = projectileMovement;
        }

        void setProjectileEntity(const ProjectileEntity& projectileEntity)
        {
            m_projectileEntity = projectileEntity;
        }

        void die() { m_die = true; }

        void alive() { m_die = false; }

        bool isDie() const { return m_die; }

        CollisionCircle getCollisionCircle() const
        {
            return m_collisionCircle;
        }

    private:
        void update_model_matrix()
        {
            m_modelMatrix = glm::identity<glm::mat4>();
            m_modelMatrix =
                glm::translate(m_modelMatrix, glm::vec3(m_projectileEntity.position, 0));
            m_modelMatrix = glm::rotate(m_modelMatrix,
                                        glm::radians(m_projectileEntity.rotation),
                                        glm::vec3(0.0f, 0.0f, 1.0f));
            m_modelMatrix =
                glm::scale(m_modelMatrix,
                           glm::vec3(m_projectileEntity.scale, m_projectileEntity.scale, 1));
        }

        void update_position(float elapsedTime)
        {
            m_projectileEntity.position +=
                m_projectileMovement.direction * m_projectileMovement.speed * elapsedTime;
            m_collisionCircle.center = m_projectileEntity.position;
            update_model_matrix();
        }

//        template<typename T>
//        void update_(float elapsedTime, T collisionObject)
//        {
//            update_position(elapsedTime);
//
//            if (collisionObject->isDie() ||
//                collisionObject.get() == this)
//                return;
//
//            auto collisionCircle = collisionObject->getCollisionCircle();
//            if (checkCollision(m_collisionCircle, collisionCircle))
//            {
//                collisionObject->die();
//                die();
//            }
//        }
//
//        template<typename IteratorT>
//        void update_(float elapsedTime, IteratorT begin, IteratorT end)
//        {
//            update_position(elapsedTime);
//
//            for (; begin != end; ++begin)
//            {
//                if (begin->isDie() ||
//                    *begin == this)
//                    continue;
//
//                auto collisionCircle = begin->getCollide();
//                if (checkCollision(m_collisionCircle, collisionCircle))
//                {
//                    begin->die();
//                    die();
//                }
//            }
//        }

    private:
        std::shared_ptr<selyan::Sprite> m_sprite;

        ProjectileMovement m_projectileMovement;
        ProjectileEntity m_projectileEntity;

        glm::mat4 m_modelMatrix;

        CollisionCircle m_collisionCircle;

        bool m_die;
    };
}
