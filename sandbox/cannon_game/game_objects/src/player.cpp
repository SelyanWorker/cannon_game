#include "game_objects/player.h"

namespace cannon_game
{
    Player::Player(const selyan::Sprite &bodySprite,
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

    void Player::update(float elapsedTime)
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
        // moveToTarget(elapsedTime);
    }

    bool Player::hasAmmo() const { return m_currentAmmo != 0; }

    void Player::decreaseAmmo()
    {
        if (m_currentAmmo == 0)
            return;

        --m_currentAmmo;
    }

    uint32_t Player::ammo() const { return m_currentAmmo; }

    void Player::draw(std::shared_ptr<selyan::Shader> shader)
    {
        assert(shader != nullptr);

        shader->setUniform("modelMatrix", glm::identity<glm::mat4>());
        m_bodySprite.draw(shader);
        shader->setUniform("modelMatrix", glm::transpose(getModelMatrix()));
        m_headSprite.draw(shader);
    }

    void Player::reset()
    {
        alive();
        m_currentAmmo = m_maxAmmo;
        m_elapsedAmmoRecoveryTime = 0;
        setRotation(0);
    }

    void Player::setMaxAmmo(uint32_t maxAmmo) { m_maxAmmo = maxAmmo; }
}