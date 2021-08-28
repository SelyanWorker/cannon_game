#pragma once

#include "game_object.h"

namespace cannon_game
{
    class Explosion : public GameObject
    {
    public:
        Explosion(const selyan::Sprite &sprite,
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

        void draw(selyan::Shader *shader) override
        {
            assert(shader != nullptr);

            shader->setUniform("modelMatrix", glm::transpose(getModelMatrix()));
            m_sprite.draw(shader);
        }

        void update(float elapsedTime)
        {
            m_sprite.update(elapsedTime);
        }

        bool animationDone() const
        {
            return m_sprite.animationDone();
        }

    private:
        selyan::Sprite m_sprite;
    };
}

class ExplosionManager
    {
    using ExplosionPtrType = std::shared_ptr<cannon_game::Explosion>;

    public:
        explicit ExplosionManager(std::shared_ptr<selyan::SpriteGeometry> spriteGeometry,
                                  std::shared_ptr<selyan::SpriteSheet> spriteSheet)
                                  : m_spriteGeometry(std::move(spriteGeometry)),
                                  m_spriteSheet(std::move(spriteSheet))
                                  {
                                  }

                                  void createExplosion(const glm::vec2 &position, float rotation, const glm::vec2 &scale)
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

                                  void update(float elapsedTime)
                                  {
            for(auto iter = m_alive.begin(); iter != m_alive.end(); )
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

                                  void draw(selyan::Shader* shader)
                                  {
            for(auto& explosion : m_alive)
            {
                explosion->draw(shader);
            }
                                  }

                                  ExplosionPtrType &operator[](size_t i) { return m_alive[i]; }

                                  std::vector<ExplosionPtrType>::iterator begin() { return m_alive.begin(); }

                                  std::vector<ExplosionPtrType>::iterator end() { return m_alive.end(); }

    private:
        static void reset(ExplosionPtrType &explosionPtr,
                          const glm::vec2 &position,
                          float rotation,
                          const glm::vec2 &scale)
                          {
            explosionPtr->setPosition(position);
            explosionPtr->setRotation(rotation);
            explosionPtr->setScale(scale);
            explosionPtr->alive();
                          }

                          selyan::Sprite createSprite()
                          {
            // TODO: add gen sprite function to sprite sheet: createSprite(begin(row, column), end(row, column))
            constexpr float frameLifeTime = 0.085;
            selyan::Sprite sprite(m_spriteGeometry, m_spriteSheet, { 0, 0, frameLifeTime });
            for (size_t i = 1; i < m_spriteSheet->getColumnsNumber(); ++i)
            {
                sprite.addSpriteFrame(1, i, frameLifeTime);
            }
            return sprite;
                          }

    private:
        std::shared_ptr<selyan::SpriteGeometry> m_spriteGeometry;
        std::shared_ptr<selyan::SpriteSheet> m_spriteSheet;

        std::vector<ExplosionPtrType> m_alive;
        std::stack<ExplosionPtrType> m_dead;
    };
