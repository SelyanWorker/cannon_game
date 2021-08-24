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
        Player(uint32_t uniqueId)
        :   GameObject(/*uniqueId*/)
        {
        }

        void update(float elapsedTime)
        {
            moveToTarget(elapsedTime);
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
        Movement m_movement;

        std::shared_ptr<GameObject> m_targetObject;
    };
}
