#pragma once

#include "game_object.h"

#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <utility>
#include <list>
#include <stack>

namespace cannon_game
{
    class Projectile : public GameObject
    {
    public:
        Projectile(const selyan::Sprite &sprite,
                   uint32_t parentId,
                   const glm::vec2 &direction,
                   float speed);

        void draw(selyan::Shader *shader) override;

        void update(float elapsedTime);

        uint32_t getParentId() const;

        void setParentId(uint32_t parentId);

        const glm::vec2 &getDirection() const;

        void setDirection(const glm::vec2 &mDirection);

        float getSpeed() const;

        void setSpeed(float mSpeed);

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
                                   std::shared_ptr<selyan::SpriteSheet> spriteSheet);

        void createProjectile(uint32_t parentId,
                              const glm::vec2 &position,
                              float rotation,
                              const glm::vec2 &direction,
                              float speed,
                              const glm::vec2 &scale,
                              float collisionScale);

        AliveProjectileContainerType::iterator kill(AliveProjectileContainerType::iterator iter);

        void removeAll();

        AliveProjectileContainerType::iterator begin();

        AliveProjectileContainerType::iterator end();

        ProjectilePtrType& back();

    private:
        std::shared_ptr<selyan::SpriteGeometry> m_spriteGeometry;
        std::shared_ptr<selyan::SpriteSheet> m_spriteSheet;

        std::stack<ProjectilePtrType> m_dead;
        std::list<ProjectilePtrType> m_alive;
    };
}
