#pragma once

#include "game_object.h"

namespace cannon_game
{
    class Enemy : public GameObject
    {
    public:
        using ShootingFunctionType =
            std::function<void(uint32_t parentId, const glm::vec2 &position, float parentRotation)>;

    public:
        Enemy(uint32_t uniqueId,
              const glm::vec2 &playerPosition,
              float spawnRadius,
              float angularVelocity,
              float distanceToPlayer,
              float reloadTime)
        :   GameObject(/*uniqueId*/),
            m_playerPosition(playerPosition),
            m_distanceToPlayer(distanceToPlayer),
            m_angularVelocity(angularVelocity),
            m_lastAngle(spawnRadius),
            m_reloadTime(reloadTime)
        {
            move(0);
        }

        void update(float elapsedTime)
        {
            move(elapsedTime);

            m_lastShotTime += elapsedTime;
            if (m_lastShotTime >= m_reloadTime)
            {
                m_shootingFun(getUniqueId(), getPosition(), getRotation());
                m_lastShotTime = 0;
            }
        }

        void setShootingFunction(const ShootingFunctionType& shootingFun)
        {
            m_shootingFun = shootingFun;
        }

        float getMAngularVelocity() const { return m_angularVelocity; }

        void setMReloadTime(float mReloadTime) { m_reloadTime = mReloadTime; }
        float getMReloadTime() const { return m_reloadTime; }

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
            //std::cout << "Enemy rotation: " << getRotation() << std::endl;
            //            auto collision = getCollision
            //            .center = newPosition;
            auto collision = getCollision();
            collision.center = newPosition;
            setCollision(collision);
        }

    private:
        glm::vec2 m_playerPosition;
        float m_distanceToPlayer;
        float m_angularVelocity;
        float m_lastAngle;

    private:
        ShootingFunctionType m_shootingFun;
        float m_reloadTime;
        float m_lastShotTime;
    };
}
