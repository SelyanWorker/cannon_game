#pragma once

#include "game_object.h"

#include <stack>

namespace cannon_game
{
    class Explosion : public GameObject
    {
    public:
        Explosion(const selyan::Sprite &sprite,
                  const glm::vec2 &position,
                  float rotation,
                  const glm::vec2 &scale);

        void draw(std::shared_ptr<selyan::Shader> shader) override;

        void update(float elapsedTime);

        bool animationDone() const;

    private:
        selyan::Sprite m_sprite;
    };

    class ExplosionManager
    {
        using ExplosionPtrType = std::shared_ptr<cannon_game::Explosion>;

    public:
        explicit ExplosionManager(std::shared_ptr<selyan::SpriteGeometry> spriteGeometry,
                                  std::shared_ptr<selyan::SpriteSheet> spriteSheet);

        void createExplosion(const glm::vec2 &position, float rotation, const glm::vec2 &scale);

        void update(float elapsedTime);

        void draw(std::shared_ptr<selyan::Shader> shader);

        void removeAll();

        ExplosionPtrType &operator[](size_t i);

        std::vector<ExplosionPtrType>::iterator begin();

        std::vector<ExplosionPtrType>::iterator end();

    private:
        static void reset(ExplosionPtrType &explosionPtr,
                          const glm::vec2 &position,
                          float rotation,
                          const glm::vec2 &scale);

        selyan::Sprite createSprite();

    private:
        std::shared_ptr<selyan::SpriteGeometry> m_spriteGeometry;
        std::shared_ptr<selyan::SpriteSheet> m_spriteSheet;

        std::vector<ExplosionPtrType> m_alive;
        std::stack<ExplosionPtrType> m_dead;
    };

}
