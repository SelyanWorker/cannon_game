#pragma once

#include "core/time.h"
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
              float reloadTime);

        void draw(selyan::Shader *shader) override;

        void update(float elapsedTime);

        void setShootingFunction(const ShootingFunctionType &shootingFun);

        float getAngularVelocity() const;

        void setAngularVelocity(float angularVelocity);

        void setReloadTime(float mReloadTime);

        float getReloadTime() const;

        float getRadius() const;

        void setRadius(float radius);

        float getAngle() const;

        void setAngle(float angle);

    private:
        void move(float elapsedTime);

    private:
        selyan::Sprite m_bodySprite;
        selyan::Sprite m_headSprite;

        glm::vec2 m_playerPosition;
        float m_radius;
        float m_angularVelocity;
        float m_angle;

        ShootingFunctionType m_shootingFun;
        float m_reloadTime;
        float m_lastShotTime;
    };

    struct EnemyGeneratorParams
    {
        float orbitHeight;
        float minAngleDifferenceBetweenTwoEnemies;
        float spawnRadiusMin;
        float spawnRadiusMax;
        float angularVelocityStep;
        float angularVelocityMin;
        float angularVelocityMax;
    };

    struct EnemyData
    {
        float radius;
        float angle;
        float angularVelocity;
    };

    class EnemiesManager
    {
        using EnemyPtrType = std::shared_ptr<Enemy>;
        using AliveEnemyContainerType = std::map<uint32_t, EnemyPtrType>;

    public:
        using ShootCallbackType = std::function<
            void(uint32_t shooterId, const glm::vec2 &shooterPosition, float shooterRotation)>;

        explicit EnemiesManager(const EnemyGeneratorParams &enemyGeneratorParams,
                                std::shared_ptr<selyan::SpriteGeometry> bodySpriteGeometry,
                                std::shared_ptr<selyan::SpriteSheet> bodySpriteSheet,
                                std::shared_ptr<selyan::SpriteGeometry> headSpriteGeometry,
                                std::shared_ptr<selyan::SpriteSheet> headSpriteSheet);

        AliveEnemyContainerType::iterator kill(AliveEnemyContainerType::iterator toEraseIter);

        auto createEnemy(const glm::vec2 &center,
                         float spawnAngle,
                         float angularVelocity,
                         float radius,
                         float reloadTime,
                         ShootCallbackType shootCallback);

        void createEnemy(const glm::vec2 &center, float reloadTime, ShootCallbackType shootCallback);

        void setEnemyGeneratorParams(const EnemyGeneratorParams &enemyGeneratorParams);

        void removeAll();

        size_t countOfAlive() const;

        std::map<uint32_t, EnemyPtrType>::iterator begin();

        std::map<uint32_t, EnemyPtrType>::iterator end();

    private:
        uint32_t calcOrbit(float radius) const;

        EnemyData generateRandomEnemyData();

        bool generateEnemyData_(EnemyData &dest);

        bool tryToCreateEnemyData(uint32_t orbit, EnemyData &enemyData);

        float calcRandAngularVelocity(float min, float max, float step);

    private:
        std::map<uint32_t, std::vector<uint32_t>> m_enemiesLocation;

        EnemyGeneratorParams m_enemyGeneratorParams;

        std::shared_ptr<selyan::SpriteSheet> m_bodySpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_bodySpriteGeometry;
        std::shared_ptr<selyan::SpriteSheet> m_headSpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_headSpriteGeometry;

        std::stack<EnemyPtrType> m_dead;
        std::map<uint32_t, EnemyPtrType> m_alive;
    };
}
