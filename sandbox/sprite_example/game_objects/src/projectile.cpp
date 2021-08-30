#include "game_objects/projectile.h"

namespace cannon_game
{
    Projectile::Projectile(const selyan::Sprite &sprite,
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

    void Projectile::draw(selyan::Shader *shader)
    {
        assert(shader != nullptr);

        shader->setUniform("modelMatrix", glm::transpose(getModelMatrix()));
        m_sprite.draw(shader);
    }

    void Projectile::update(float elapsedTime)
    {
        auto position = getPosition();
        position += m_direction * m_speed * elapsedTime;
        setPosition(position);
    }

    uint32_t Projectile::getParentId() const { return m_parentId; }

    void Projectile::setParentId(uint32_t parentId) { m_parentId = parentId; }

    const glm::vec2 &Projectile::getDirection() const { return m_direction; }

    void Projectile::setDirection(const glm::vec2 &mDirection) { m_direction = mDirection; }

    float Projectile::getSpeed() const { return m_speed; }

    void Projectile::setSpeed(float mSpeed) { m_speed = mSpeed; }

    ProjectileManager::ProjectileManager(std::shared_ptr<selyan::SpriteGeometry> spriteGeometry,
                                         std::shared_ptr<selyan::SpriteSheet> spriteSheet)
      : m_spriteGeometry(std::move(spriteGeometry)),
        m_spriteSheet(std::move(spriteSheet))
    {
    }

    void ProjectileManager::createProjectile(uint32_t parentId,
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
        auto projectile =
            std::make_shared<cannon_game::Projectile>(projectileSprite, parentId, direction, speed);
        projectile->setPosition(position);
        projectile->setRotation(rotation);
        projectile->setScale(scale);
        projectile->setCollision({ position, collisionScale });
        m_alive.push_back(projectile);
    }

    ProjectileManager::AliveProjectileContainerType::iterator ProjectileManager::kill(
        AliveProjectileContainerType::iterator iter)
    {
        (*iter)->die();
        m_dead.push(*iter);
        return m_alive.erase(iter);
    }

    void ProjectileManager::removeAll() { m_alive.clear(); }

    ProjectileManager::AliveProjectileContainerType::iterator ProjectileManager::begin()
    {
        return m_alive.begin();
    }

    ProjectileManager::AliveProjectileContainerType::iterator ProjectileManager::end()
    {
        return m_alive.end();
    }

    ProjectileManager::ProjectilePtrType &ProjectileManager::back() { return m_alive.back(); }

}