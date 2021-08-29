#pragma once

#include "collision.h"

#include "game_objects/game_object.h"

namespace cannon_game
{
//    struct Movement
//    {
//        float angleOffset;
//        float speed;
//    };

    class Player : public GameObject
    {
    public:
        Player(const selyan::Sprite &bodySprite,
               const selyan::Sprite &headSprite,
               float ammoRecoveryTime,
               uint32_t maxAmmo)
          : GameObject(),
            m_bodySprite(bodySprite),
            m_headSprite(headSprite),
            m_ammoRecoveryTime(ammoRecoveryTime),
            m_maxAmmo(maxAmmo),
            m_currentAmmo(maxAmmo),
            m_elapsedAmmoRecoveryTime(0)
        {
        }

        void update(float elapsedTime)
        {
            m_headSprite.update(elapsedTime);
            m_bodySprite.update(elapsedTime);

            if (m_currentAmmo != m_maxAmmo)
            {
                m_elapsedAmmoRecoveryTime += elapsedTime;
                if (m_elapsedAmmoRecoveryTime >= m_ammoRecoveryTime)
                {
                    ++m_currentAmmo;
                    m_elapsedAmmoRecoveryTime = 0;
                }
            }
            //moveToTarget(elapsedTime);
        }

        bool hasAmmo() const
        {
            return m_currentAmmo != 0;
        }

        void decreaseAmmo()
        {
            if(m_currentAmmo == 0)
                return;

            --m_currentAmmo;
        }

        uint32_t ammo() const
        {
            return m_currentAmmo;
        }

        void draw(selyan::Shader *shader) override
        {
            assert(shader != nullptr);

            shader->setUniform("modelMatrix", glm::identity<glm::mat4>());
            m_bodySprite.draw(shader);
            shader->setUniform("modelMatrix", glm::transpose(getModelMatrix()));
            m_headSprite.draw(shader);
        }

        void reset()
        {
            alive();
            m_currentAmmo = m_maxAmmo;
            m_elapsedAmmoRecoveryTime = 0;
            setRotation(0);
        }

        void setMaxAmmo(uint32_t maxAmmo)
        {
            m_maxAmmo = maxAmmo;
        }

    private:
        selyan::Sprite m_bodySprite;
        selyan::Sprite m_headSprite;

        float m_ammoRecoveryTime;
        uint32_t m_maxAmmo;
        uint32_t m_currentAmmo;
        float m_elapsedAmmoRecoveryTime;
    };
}
