#pragma once

#include <memory>

namespace test
{
    class GameObject
    {
    public:
        GameObject()
            : dead(false)
            {}

        virtual ~GameObject() = default;

        void die()
        {
            dead = true;
        }

        bool isDead() const
        {
            return dead;
        }

    private:
        bool dead = false;
    };

    class Enemy : public GameObject
    {
    public:
        ~Enemy() = default;
    };

    class Projectile : public GameObject
    {
    public:
        ~Projectile() = default;
    };

    void testFun(std::shared_ptr<Projectile> projectile, std::shared_ptr<GameObject> gameObject)
    {
        gameObject->die();
    }
}
