#include "game_objects/explosion.h"

namespace cannon_game
{
    Explosion::Explosion(const selyan::Sprite &sprite,
                         const glm::vec2 &position,
                         float rotation,
                         const glm::vec2 &scale)
      : GameObject(),
        m_sprite(sprite)
    {
        setPosition(position);
        setRotation(rotation);
        setScale(scale);
    }

    void Explosion::draw(selyan::Shader *shader)
    {
        assert(shader != nullptr);

        shader->setUniform("modelMatrix", glm::transpose(getModelMatrix()));
        m_sprite.draw(shader);
    }

    void Explosion::update(float elapsedTime) { m_sprite.update(elapsedTime); }

    bool Explosion::animationDone() const { return m_sprite.animationDone(); }

    ExplosionManager::ExplosionManager(std::shared_ptr<selyan::SpriteGeometry> spriteGeometry,
                                       std::shared_ptr<selyan::SpriteSheet> spriteSheet)
      : m_spriteGeometry(std::move(spriteGeometry)),
        m_spriteSheet(std::move(spriteSheet))
    {
    }

    void ExplosionManager::createExplosion(const glm::vec2 &position,
                                           float rotation,
                                           const glm::vec2 &scale)
    {
        if (!m_dead.empty())
        {
            auto deadExplosion = m_dead.top();
            m_dead.pop();
            reset(deadExplosion, position, rotation, scale);
            m_alive.push_back(deadExplosion);
            return;
        }

        m_alive.push_back(
            std::make_shared<cannon_game::Explosion>(createSprite(), position, rotation, scale));
    }

    void ExplosionManager::update(float elapsedTime)
    {
        for (auto iter = m_alive.begin(); iter != m_alive.end();)
        {
            (*iter)->update(elapsedTime);

            if ((*iter)->animationDone())
            {
                m_dead.push(*iter);
                iter = m_alive.erase(iter);
            }
            else
            {
                ++iter;
            }
        }
    }

    void ExplosionManager::draw(selyan::Shader *shader)
    {
        for (auto &explosion : m_alive)
        {
            explosion->draw(shader);
        }
    }

    void ExplosionManager::removeAll() { m_alive.clear(); }

    ExplosionManager::ExplosionPtrType &ExplosionManager::operator[](size_t i)
    {
        return m_alive[i];
    }

    std::vector<ExplosionManager::ExplosionPtrType>::iterator ExplosionManager::begin()
    {
        return m_alive.begin();
    }

    std::vector<ExplosionManager::ExplosionPtrType>::iterator ExplosionManager::end()
    {
        return m_alive.end();
    }

    void ExplosionManager::reset(ExplosionPtrType &explosionPtr,
                                        const glm::vec2 &position,
                                        float rotation,
                                        const glm::vec2 &scale)
    {
        explosionPtr->setPosition(position);
        explosionPtr->setRotation(rotation);
        explosionPtr->setScale(scale);
        explosionPtr->alive();
    }

    selyan::Sprite ExplosionManager::createSprite()
    {
        // TODO: add gen sprite function to sprite sheet: createSprite(begin(row, column), end(row,
        // column))
        constexpr float frameLifeTime = 0.085;
        selyan::Sprite sprite(m_spriteGeometry, m_spriteSheet, { 0, 0, frameLifeTime });
        for (size_t i = 1; i < m_spriteSheet->getColumnsNumber(); ++i)
        {
            sprite.addSpriteFrame(1, i, frameLifeTime);
        }
        return sprite;
    }

}