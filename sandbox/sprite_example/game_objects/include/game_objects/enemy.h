#pragma once

#include "game_object.h"

#include <functional>
#include <map>
#include <stack>

namespace cannon_game
{
    class Enemy : public GameObject
    {
    public:
        using ShootingFunctionType =
            std::function<void(uint32_t parentId, const glm::vec2 &position, float parentRotation)>;

    public:
        Enemy(const selyan::Sprite &bodySprite,
              const selyan::Sprite &headSprite,
              const glm::vec2 &playerPosition,
              float spawnAngle,
              float angularVelocity,
              float distanceToPlayer,
              float reloadTime)
          : GameObject(),
            m_bodySprite(bodySprite),
            m_headSprite(headSprite),
            m_playerPosition(playerPosition),
            m_distanceToPlayer(distanceToPlayer),
            m_angularVelocity(angularVelocity),
            m_lastAngle(spawnAngle),
            m_reloadTime(reloadTime),
            m_lastShotTime(0)
        {
            move(0);
        }

        void draw(selyan::Shader *shader) override
        {
            assert(shader != nullptr);

            auto modelMatrix = getModelMatrix();

            constexpr float absoluteHeadRotation = 90.f;
            float headRotation =
                m_angularVelocity <= 0 ? -absoluteHeadRotation : absoluteHeadRotation;
            auto headModelMatrix =
                glm::rotate(modelMatrix, glm::radians(headRotation), glm::vec3(0.0f, 0.0f, 1.0f));

            shader->setUniform("modelMatrix", glm::transpose(modelMatrix));
            m_bodySprite.draw(shader);

            shader->setUniform("modelMatrix", glm::transpose(headModelMatrix));
            m_headSprite.draw(shader);
        }

        void update(float elapsedTime)
        {
            move(elapsedTime);

            m_bodySprite.update(elapsedTime);
            m_headSprite.update(elapsedTime);

            m_lastShotTime += elapsedTime;
            if (m_lastShotTime >= m_reloadTime)
            {
                m_shootingFun(getUniqueId(), getPosition(), getRotation());
                m_lastShotTime = 0;
            }
        }

        void setShootingFunction(const ShootingFunctionType &shootingFun)
        {
            m_shootingFun = shootingFun;
        }

        float getMAngularVelocity() const { return m_angularVelocity; }

        void setMReloadTime(float mReloadTime) { m_reloadTime = mReloadTime; }
        float getMReloadTime() const { return m_reloadTime; }
        float getMDistanceToPlayer() const { return m_distanceToPlayer; }

        float getMLastAngle() const { return m_lastAngle; }

    private:
        void move(float elapsedTime)
        {
            float phi = glm::radians(m_lastAngle) - elapsedTime;

            m_lastAngle += m_angularVelocity * elapsedTime;
            if (m_lastAngle >= 360.)
                m_lastAngle -= 360.;
            auto newPosition = m_playerPosition + glm::vec2(m_distanceToPlayer * glm::cos(phi),
                                                            m_distanceToPlayer * glm::sin(phi));

            setPosition(newPosition);
            setRotation(m_lastAngle);
            // std::cout << "Enemy rotation: " << getRotation() << std::endl;
            //             auto collision = getCollision
            //             .center = newPosition;
            auto collision = getCollision();
            collision.center = newPosition;
            setCollision(collision);
        }

    private:
        selyan::Sprite m_bodySprite;
        selyan::Sprite m_headSprite;

        glm::vec2 m_playerPosition;
        float m_distanceToPlayer;
        float m_angularVelocity;
        float m_lastAngle;

        ShootingFunctionType m_shootingFun;
        float m_reloadTime;
        float m_lastShotTime;
    };

    class EnemiesManager
    {
        using EnemyPtrType = std::shared_ptr<Enemy>;

    public:
        using ShootCallbackType = std::function<void(uint32_t shooterId,
                                                     const glm::vec2 &shooterPosition,
                                                     float shooterRotation)>;

        explicit EnemiesManager(std::shared_ptr<selyan::SpriteGeometry> bodySpriteGeometry,
                                std::shared_ptr<selyan::SpriteSheet> bodySpriteSheet,
                                std::shared_ptr<selyan::SpriteGeometry> headSpriteGeometry,
                                std::shared_ptr<selyan::SpriteSheet> headSpriteSheet)
          : m_bodySpriteGeometry(std::move(bodySpriteGeometry)),
            m_bodySpriteSheet(std::move(bodySpriteSheet)),
            m_headSpriteGeometry(std::move(headSpriteGeometry)),
            m_headSpriteSheet(std::move(headSpriteSheet))
        {
        }

        void kill(uint32_t id)
        {
            auto found = m_alive.find(id);

            if (found == m_alive.end())
                return;

            found->second->die();
            m_dead.push(found->second);
        }

        void createEnemy(const glm::vec2 &center,
                         float spawnAngle,
                         float angularVelocity,
                         float radius,
                         float reloadTime,
                         ShootCallbackType shootCallback)
        {
            selyan::Sprite enemyBodySprite(m_bodySpriteGeometry,
                                           m_bodySpriteSheet,
                                           selyan::SpriteFrame{ 0, 0, 0.085 });
            selyan::Sprite enemyHeadSprite(m_headSpriteGeometry,
                                           m_headSpriteSheet,
                                           selyan::SpriteFrame{ 0, 0, 0.085 });
            auto enemy = std::make_shared<cannon_game::Enemy>(enemyBodySprite,
                                                              enemyHeadSprite,
                                                              center,
                                                              spawnAngle,
                                                              angularVelocity,
                                                              radius,
                                                              reloadTime);
            enemy->setScale({ 1, 1 });
            enemy->setCollision({ enemy->getPosition(), 0.5f });
            enemy->setShootingFunction(shootCallback);
            m_alive.insert(std::make_pair(enemy->getUniqueId(), enemy));
        }

        std::map<uint32_t, EnemyPtrType>::iterator begin()
        {
            return m_alive.begin();
        }

        std::map<uint32_t, EnemyPtrType>::iterator end()
        {
            return m_alive.end();
        }

    private:
        std::shared_ptr<selyan::SpriteSheet> m_bodySpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_bodySpriteGeometry;
        std::shared_ptr<selyan::SpriteSheet> m_headSpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_headSpriteGeometry;

        std::stack<EnemyPtrType> m_dead;
        std::map<uint32_t, EnemyPtrType> m_alive;
    };
}
