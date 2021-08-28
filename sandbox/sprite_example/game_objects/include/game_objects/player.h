#pragma once

#include "collision.h"

#include "game_objects/game_object.h"

namespace cannon_game
{
    struct Movement
    {
        float angleOffset;
        float speed;
    };

    class Player : public GameObject
    {
    public:
        Player(const selyan::Sprite &bodySprite, const selyan::Sprite &headSprite)
          : GameObject(),
            m_bodySprite(bodySprite),
            m_headSprite(headSprite)
        {
        }

        void update(float elapsedTime)
        {
            m_headSprite.update(elapsedTime);
            m_bodySprite.update(elapsedTime);

            moveToTarget(elapsedTime);
        }

        void draw(selyan::Shader *shader) override
        {
            assert(shader != nullptr);

            shader->setUniform("modelMatrix", glm::identity<glm::mat4>());
            m_bodySprite.draw(shader);
            shader->setUniform("modelMatrix", glm::transpose(getModelMatrix()));
            m_headSprite.draw(shader);
        }

        const Movement &getMovement() const { return m_movement; }

        void setMovement(const Movement &mMovement) { m_movement = mMovement; }

        void setMTargetObject(const std::shared_ptr<GameObject> &mTargetObject)
        {
            m_targetObject = mTargetObject;
        }

    private:
        void moveToTarget(float elapsedTime)
        {
            if (m_targetObject == nullptr || m_targetObject->isDie())
                return;

            auto rotation = getRotation();
            if (rotation <= (m_targetObject->getRotation() + m_movement.angleOffset))
            {
                rotation += elapsedTime * m_movement.speed;
                setRotation(rotation);
            }
        }

    private:
        selyan::Sprite m_bodySprite;
        selyan::Sprite m_headSprite;

        Movement m_movement;

        std::shared_ptr<GameObject> m_targetObject;
    };
}
