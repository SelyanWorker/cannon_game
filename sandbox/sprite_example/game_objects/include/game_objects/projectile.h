#pragma once

#include "game_object.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <utility>
#include <list>

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

        void draw(selyan::Shader *shader) override
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

        uint32_t getParentId() const { return m_parentId; }

        void setParentId(uint32_t parentId) { m_parentId = parentId; }

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

    class ProjectileManager
    {
        using ProjectilePtrType = std::shared_ptr<Projectile>;
        using AliveProjectileContainerType = std::list<ProjectilePtrType>;

    public:
        explicit ProjectileManager(std::shared_ptr<selyan::SpriteGeometry> spriteGeometry,
                                   std::shared_ptr<selyan::SpriteSheet> spriteSheet)
          : m_spriteGeometry(std::move(spriteGeometry)),
            m_spriteSheet(std::move(spriteSheet))
        {
        }

        void createProjectile(uint32_t parentId,
                              const glm::vec2 &position,
                              float rotation,
                              const glm::vec2 &direction,
                              float speed,
                              const glm::vec2 &scale,
                              float collisionScale)
        {
            if (!m_dead.empty())
            {
                auto dead = m_dead.top();
                m_dead.pop();
                dead->alive();
                dead->setParentId(parentId);
                dead->setPosition(position);
                dead->setRotation(rotation);
                dead->setDirection(direction);
                dead->setSpeed(speed);
                dead->setScale(scale);
                dead->setCollision({ position, collisionScale });
                m_alive.push_back(dead);
                return;
            }

            selyan::Sprite projectileSprite(m_spriteGeometry, m_spriteSheet);
            auto projectile = std::make_shared<cannon_game::Projectile>(projectileSprite,
                                                                        parentId,
                                                                        direction,
                                                                        speed);
            projectile->setPosition(position);
            projectile->setRotation(rotation);
            projectile->setScale(scale);
            projectile->setCollision({ position, collisionScale });
            //m_alive.insert(std::make_pair(projectile->getUniqueId(), projectile));
            m_alive.push_back(projectile);
        }

        //        void kill(uint32_t id)
        //        {
        //            auto found = m_alive.find(id);
        //
        //            if (found == m_alive.end())
        //                return;
        //
        //            found->second->die();
        //            m_dead.push(found->second);
        //            m_alive.erase(found);
        //        }

        AliveProjectileContainerType::iterator kill(AliveProjectileContainerType::iterator iter)
        {
//            auto found = m_alive.find(id);
//
//            if (found == m_alive.end())
//                return;

            (*iter)->die();
            m_dead.push(*iter);
            return m_alive.erase(iter);
        }

        AliveProjectileContainerType::iterator begin() { return m_alive.begin(); }

        AliveProjectileContainerType::iterator end() { return m_alive.end(); }

    private:
        std::shared_ptr<selyan::SpriteGeometry> m_spriteGeometry;
        std::shared_ptr<selyan::SpriteSheet> m_spriteSheet;

        std::stack<ProjectilePtrType> m_dead;
        std::list<ProjectilePtrType> m_alive;
    };
}
