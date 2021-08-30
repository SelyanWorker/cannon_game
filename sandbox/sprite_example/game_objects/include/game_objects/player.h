#pragma once

#include "collision.h"

#include "game_objects/game_object.h"

namespace cannon_game
{
    class Player : public GameObject
    {
    public:
        Player(const selyan::Sprite &bodySprite,
               const selyan::Sprite &headSprite,
               float ammoRecoveryTime,
               uint32_t maxAmmo);

        void update(float elapsedTime);

        bool hasAmmo() const;

        void decreaseAmmo();

        uint32_t ammo() const;

        void draw(selyan::Shader *shader) override;

        void reset();

        void setMaxAmmo(uint32_t maxAmmo);

    private:
        selyan::Sprite m_bodySprite;
        selyan::Sprite m_headSprite;

        float m_ammoRecoveryTime;
        uint32_t m_maxAmmo;
        uint32_t m_currentAmmo;
        float m_elapsedAmmoRecoveryTime;
    };
}
