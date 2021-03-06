#include "game_objects/enemy.h"

namespace cannon_game
{
    Enemy::Enemy(const selyan::Sprite &bodySprite,
                 const selyan::Sprite &headSprite,
                 const glm::vec2 &playerPosition,
                 float spawnAngle,
                 float angularVelocity,
                 float distanceToPlayer,
                 float reloadTime)
      : GameObject(),
        m_bodySprite(bodySprite),
        m_headSprite(headSprite),
        m_playerPosition(playerPosition),
        m_radius(distanceToPlayer),
        m_angularVelocity(angularVelocity),
        m_angle(spawnAngle),
        m_reloadTime(reloadTime),
        m_lastShotTime(0)
    {
        move(0);
    }

    void Enemy::draw(std::shared_ptr<selyan::Shader> shader)
    {
        assert(shader != nullptr);

        auto modelMatrix = getModelMatrix();

        constexpr float absoluteHeadRotation = 90.f;
        float headRotation = m_angularVelocity <= 0 ? -absoluteHeadRotation : absoluteHeadRotation;
        auto headModelMatrix =
            glm::rotate(modelMatrix, glm::radians(headRotation), glm::vec3(0.0f, 0.0f, 1.0f));

        shader->setUniform("modelMatrix", glm::transpose(modelMatrix));
        m_bodySprite.draw(shader);

        shader->setUniform("modelMatrix", glm::transpose(headModelMatrix));
        m_headSprite.draw(shader);
    }

    void Enemy::update(float elapsedTime)
    {
        move(elapsedTime);

        m_bodySprite.update(elapsedTime);
        m_headSprite.update(elapsedTime);

        m_lastShotTime += elapsedTime;
        if (m_lastShotTime >= m_reloadTime)
        {
            m_shootingFun(getUniqueId(), getPosition(), getRotation());
            m_lastShotTime = 0;
        }
    }

    void Enemy::setShootingFunction(const ShootingFunctionType &shootingFun)
    {
        m_shootingFun = shootingFun;
    }

    float Enemy::getAngularVelocity() const { return m_angularVelocity; }

    void Enemy::setAngularVelocity(float angularVelocity) { m_angularVelocity = angularVelocity; }

    void Enemy::setReloadTime(float mReloadTime) { m_reloadTime = mReloadTime; }

    float Enemy::getReloadTime() const { return m_reloadTime; }

    float Enemy::getRadius() const { return m_radius; }

    void Enemy::setRadius(float radius)
    {
        m_radius = radius;
        move(0);
    }

    float Enemy::getAngle() const { return m_angle; }

    void Enemy::setAngle(float angle)
    {
        m_angle = angle;
        move(0);
    }

    void Enemy::move(float elapsedTime)
    {
        float phi = glm::radians(m_angle) - elapsedTime;

        m_angle += m_angularVelocity * elapsedTime;
        if (m_angle >= 360.)
            m_angle -= 360.;
        auto newPosition =
            m_playerPosition + glm::vec2(m_radius * glm::cos(phi), m_radius * glm::sin(phi));

        setPosition(newPosition);
        setRotation(m_angle);
        // std::cout << "Enemy rotation: " << getRotation() << std::endl;
        //             auto collision = getCollision
        //             .center = newPosition;
        auto collision = getCollision();
        collision.center = newPosition;
        setCollision(collision);
    }

    EnemiesManager::EnemiesManager(const EnemyGeneratorParams &enemyGeneratorParams,
                                   std::shared_ptr<selyan::SpriteGeometry> bodySpriteGeometry,
                                   std::shared_ptr<selyan::SpriteSheet> bodySpriteSheet,
                                   std::shared_ptr<selyan::SpriteGeometry> headSpriteGeometry,
                                   std::shared_ptr<selyan::SpriteSheet> headSpriteSheet)
      : m_enemyGeneratorParams(enemyGeneratorParams),
        m_bodySpriteSheet(std::move(bodySpriteSheet)),
        m_bodySpriteGeometry(std::move(bodySpriteGeometry)),
        m_headSpriteSheet(std::move(headSpriteSheet)),
        m_headSpriteGeometry(std::move(headSpriteGeometry))
    {
    }

    EnemiesManager::AliveEnemyContainerType::iterator EnemiesManager::kill(
        AliveEnemyContainerType::iterator toEraseIter)
    {
        if (toEraseIter == m_alive.end())
            return m_alive.end();

        toEraseIter->second->die();
        m_dead.push(toEraseIter->second);

        auto &enemiesOnThisOrbit = m_enemiesLocation.at(
            uint32_t(float(toEraseIter->second->getRadius() - m_enemyGeneratorParams.spawnRadiusMin) /
            m_enemyGeneratorParams.orbitHeight));

        auto toRemove = std::find_if(enemiesOnThisOrbit.begin(),
                                     enemiesOnThisOrbit.end(),
                                     [&](const auto &item)
                                     { return toEraseIter->second->getUniqueId() == item; });
        if (toRemove != enemiesOnThisOrbit.end())
            enemiesOnThisOrbit.erase(toRemove);

        return m_alive.erase(toEraseIter);
    }

    auto EnemiesManager::createEnemy(const glm::vec2 &center,
                                     float spawnAngle,
                                     float angularVelocity,
                                     float radius,
                                     float reloadTime,
                                     const ShootCallbackType& shootCallback)
    {
        if (!m_dead.empty())
        {
            auto deadEnemy = m_dead.top();
            m_dead.pop();
            deadEnemy->alive();
            deadEnemy->setAngle(spawnAngle);
            deadEnemy->setAngularVelocity(angularVelocity);
            deadEnemy->setRadius(radius);
            deadEnemy->setReloadTime(reloadTime);
            deadEnemy->setShootingFunction(shootCallback);
            m_alive.insert(std::make_pair(deadEnemy->getUniqueId(), deadEnemy));
        }

        selyan::Sprite enemyBodySprite(m_bodySpriteGeometry, m_bodySpriteSheet);
        selyan::Sprite enemyHeadSprite(m_headSpriteGeometry, m_headSpriteSheet);

        auto enemy = std::make_shared<cannon_game::Enemy>(enemyBodySprite,
                                                          enemyHeadSprite,
                                                          center,
                                                          spawnAngle,
                                                          angularVelocity,
                                                          radius,
                                                          reloadTime);
        enemy->setScale({ 1, 1 });
        enemy->setCollision({ enemy->getPosition(), 0.5f });
        enemy->setShootingFunction(shootCallback);
        return m_alive.insert(std::make_pair(enemy->getUniqueId(), enemy));
    }

    void EnemiesManager::createEnemy(const glm::vec2 &center,
                                     float reloadTime,
                                     const ShootCallbackType& shootCallback)
    {
        EnemyData enemyParams{};
        if (!generateEnemyData(enemyParams))
            return;

        auto lastEnemy = createEnemy(center,
                                     enemyParams.angle,
                                     enemyParams.angularVelocity,
                                     enemyParams.radius,
                                     reloadTime,
                                     shootCallback);
        uint32_t lastEnemyOrbit = calcOrbit(lastEnemy.first->second->getRadius());
        m_enemiesLocation[lastEnemyOrbit].push_back(lastEnemy.first->second->getUniqueId());
    }

    void EnemiesManager::setEnemyGeneratorParams(const EnemyGeneratorParams &enemyGeneratorParams)
    {
        m_enemyGeneratorParams = enemyGeneratorParams;
    }

    void EnemiesManager::removeAll()
    {
        m_alive.clear();
        m_enemiesLocation.clear();
    }

    size_t EnemiesManager::countOfAlive() const { return m_alive.size(); }

    std::map<uint32_t, EnemiesManager::EnemyPtrType>::iterator EnemiesManager::begin()
    {
        return m_alive.begin();
    }

    std::map<uint32_t, EnemiesManager::EnemyPtrType>::iterator EnemiesManager::end()
    {
        return m_alive.end();
    }

    uint32_t EnemiesManager::calcOrbit(float radius) const
    {
        return float(radius - m_enemyGeneratorParams.spawnRadiusMin) /
               m_enemyGeneratorParams.orbitHeight;
    }

    bool EnemiesManager::generateEnemyData(EnemyData &dest)
    {
        auto seed = selyan::TimeStep::getTime().getMicro();
        std::srand(uint32_t(seed));

        uint32_t maxOrbitCount = calcOrbit(m_enemyGeneratorParams.spawnRadiusMax);
        uint32_t orbit = 1 + std::rand() / ((RAND_MAX + 1u) / maxOrbitCount);

        if (tryToCreateEnemyData(orbit, dest))
            return true;

        for (auto i = orbit + 1; i <= maxOrbitCount; ++i)
        {
            if (tryToCreateEnemyData(i, dest))
                return true;
        }

        for (auto i = 0; i < orbit; ++i)
        {
            if (tryToCreateEnemyData(i, dest))
                return true;
        }

        return false;
    }

    bool EnemiesManager::tryToCreateEnemyData(uint32_t orbit, EnemyData &enemyData)
    {
        auto &enemiesOnOrbit = m_enemiesLocation[orbit];
        if (enemiesOnOrbit.empty())
        {
            uint32_t angle = 0 + std::rand() / ((RAND_MAX + 1u) / 360);

            float angularVelocity =
                calcRandAngularVelocity(m_enemyGeneratorParams.angularVelocityMin,
                                        m_enemyGeneratorParams.angularVelocityMax,
                                        m_enemyGeneratorParams.angularVelocityStep);

            enemyData = { float(orbit) * m_enemyGeneratorParams.orbitHeight +
                              m_enemyGeneratorParams.spawnRadiusMin,
                          float(angle),
                          angularVelocity };
            return true;
        }

        if (enemiesOnOrbit.size() == 1)
        {
            auto adjacent = m_alive.at(enemiesOnOrbit.front());
            enemyData = { float(orbit) * m_enemyGeneratorParams.orbitHeight +
                              m_enemyGeneratorParams.spawnRadiusMin,
                          360 - adjacent->getAngle(),
                          calcRandAngularVelocity(adjacent->getAngularVelocity(),
                                                  m_enemyGeneratorParams.angularVelocityMax,
                                                  m_enemyGeneratorParams.angularVelocityStep) };
            return true;
        }

        auto left = enemiesOnOrbit.front();
        auto right = enemiesOnOrbit.back();
        //            float distance =
        //                glm::distance2(m_alive.at(right)->getPosition(),
        //                m_alive.at(left)->getPosition());
        float angleDifference =
            glm::abs(m_alive.at(right)->getAngle() - m_alive.at(left)->getAngle());
        for (auto iter = enemiesOnOrbit.begin(); iter != std::prev(enemiesOnOrbit.end()); ++iter)
        {
            auto &leftAdjacent = m_alive.at(*iter);
            auto &rightAdjacent = m_alive.at(*std::next(iter));
            auto adjacentAngleDifference =
                glm::abs(leftAdjacent->getAngle() - rightAdjacent->getAngle());
            if (adjacentAngleDifference > angleDifference)
            {
                angleDifference = adjacentAngleDifference;
                left = *iter;
                right = *std::next(iter);
            }
        }

        if (angleDifference < m_enemyGeneratorParams.minAngleDifferenceBetweenTwoEnemies * 2)
        {
            return false;
        }

        auto &leftAdjacent = m_alive.at(left);
        auto &rightAdjacent = m_alive.at(right);
        enemyData = {
            orbit * m_enemyGeneratorParams.orbitHeight + m_enemyGeneratorParams.spawnRadiusMin,
            leftAdjacent->getAngle() + (leftAdjacent->getAngle() - rightAdjacent->getAngle()) / 2,
            calcRandAngularVelocity(leftAdjacent->getAngularVelocity(),
                                    rightAdjacent->getAngularVelocity(),
                                    m_enemyGeneratorParams.angularVelocityStep)
        };
        return true;
    }

    float EnemiesManager::calcRandAngularVelocity(float min, float max, float step)
    {
        assert(min <= max && "Invalid min more than max");

        if (min == max)
            return min;

        uint32_t angularVelocityStepCount = uint32_t((max - min) / step);
        int32_t rawAngularVelocity = 1 + std::rand() / ((RAND_MAX + 1u) / angularVelocityStepCount);
        auto angularVelocitySign = std::rand() / ((RAND_MAX + 1u));

        float angularVelocity = float(rawAngularVelocity) * step + min;
        if (angularVelocitySign)
            angularVelocity *= -1;

        return angularVelocity;
    }
}