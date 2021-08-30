#include "game_layers/main_layer.h"

namespace cannon_game
{
    namespace config
    {
        constexpr float minAngleDifferenceBetweenTwoEnemies = 10;
        // constexpr float spawnRadiusMax = 7;
        constexpr float minSpawnRadius = 4;
        constexpr float orbitSize = 1;

        constexpr float coordSystemWidth = 10.f;
    }

    namespace utility
    {
        float get_current_window_aspect_ratio()
        {
            return float(selyan::Application::get()->getWindow()->getHeight()) /
                   float(selyan::Application::get()->getWindow()->getWidth());
        }
    }

    namespace detail
    {
        inline std::shared_ptr<cannon_game::Projectile> findNearestProjectile(
            const std::shared_ptr<cannon_game::ProjectileManager> &projectileManager,
            const std::shared_ptr<cannon_game::GameObject> &gameObject,
            uint32_t prevTargetId)
        {
            if (projectileManager->begin() == projectileManager->end())
                return nullptr;

            constexpr float minDistance = 0.1f;
            auto playerPosition = gameObject->getPosition();
            std::shared_ptr<cannon_game::Projectile> nearestProjectile =
                *projectileManager->begin();
            auto nearestProjectileDistance =
                glm::length2(playerPosition - nearestProjectile->getPosition());
            for (auto i = std::next(projectileManager->begin()); i != projectileManager->end(); ++i)
            {
                if ((*i)->getParentId() == gameObject->getUniqueId() ||
                    (*i)->getUniqueId() == prevTargetId)
                    continue;

                auto projectilePosition = (*i)->getPosition();
                auto distance = glm::length2(playerPosition - projectilePosition);
                if (distance < minDistance)
                    continue;

                if (distance > nearestProjectileDistance)
                    continue;

                nearestProjectile = (*i);
                nearestProjectileDistance = distance;
            }

            if (nearestProjectile->getParentId() == gameObject->getUniqueId() ||
                nearestProjectile->getUniqueId() == prevTargetId)
                return nullptr;

            return nearestProjectile;
        }

        std::shared_ptr<cannon_game::Enemy> findNearestEnemy(
            const std::shared_ptr<cannon_game::EnemiesManager> &enemiesManager,
            const std::shared_ptr<cannon_game::GameObject> &gameObject,
            uint32_t prevTargetId)
        {
            if (enemiesManager->begin() == enemiesManager->end())
                return nullptr;

            constexpr float minDistance = 0.1f;
            auto playerPosition = gameObject->getPosition();
            std::shared_ptr<cannon_game::Enemy> nearestEnemy = enemiesManager->begin()->second;
            auto nearestEnemyDistance = glm::length2(playerPosition - nearestEnemy->getPosition());
            for (auto i = std::next(enemiesManager->begin()); i != enemiesManager->end(); ++i)
            {
                if (i->second->getUniqueId() == prevTargetId)
                    continue;

                auto projectilePosition = i->second->getPosition();
                auto distance = glm::length2(playerPosition - projectilePosition);
                if (distance < minDistance)
                    continue;   // return?

                if (distance > nearestEnemyDistance)
                    continue;

                nearestEnemy = i->second;
                nearestEnemyDistance = distance;
            }

            if (nearestEnemy->getUniqueId() == prevTargetId)
                return nullptr;

            return nearestEnemy;
        }
    }

    struct MainLayer::impl
    {
        explicit impl(const cannon_game::GameParams &initParams)
          : m_params(initParams),
            m_gameStop(false),
            m_speedUpMode(false),
            m_prevTargetId(0),
            m_currentPlayerHealth(initParams.initialPlayerHealth)
        {
            m_shader = std::shared_ptr<selyan::Shader>(
                selyan::ShaderLibrary::createShaderFromFile(R"(.\res\shaders\Sprite.glsl)"));

            auto image = selyan::Image::create(R"(.\res\enemy\Hull_02.png)");
            auto enemyBodyTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            enemyBodyTexture->textureData(image);
            delete image;
            std::shared_ptr<selyan::SpriteSheet> enemyBodySpriteSheet =
                std::make_shared<selyan::SpriteSheet>(enemyBodyTexture, 1, 1);
            std::shared_ptr<selyan::SpriteGeometry> enemyBodySpriteGeometry =
                std::make_shared<selyan::SpriteGeometry>(
                    enemyBodySpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                    enemyBodySpriteSheet->getRelativeSpriteSize(),
                    -2);

            image = selyan::Image::create(R"(.\res\enemy\Gun_01.png)");
            auto enemyHeadTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            enemyHeadTexture->textureData(image);
            delete image;
            std::shared_ptr<selyan::SpriteSheet> enemyHeadSpriteSheet =
                std::make_shared<selyan::SpriteSheet>(enemyHeadTexture, 1, 1);
            std::shared_ptr<selyan::SpriteGeometry> enemyHeadSpriteGeometry =
                std::make_shared<selyan::SpriteGeometry>(
                    enemyHeadSpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                    enemyHeadSpriteSheet->getRelativeSpriteSize(),
                    -1,
                    glm::vec2{ -0.1, 0.3 });

            EnemyGeneratorParams enemyGeneratorParams{ config::orbitSize,
                                                       config::minAngleDifferenceBetweenTwoEnemies,
                                                       config::minSpawnRadius,
                                                       m_params.maxSpawnRadius,
                                                       1,
                                                       m_params.enemyAngularVelocityMin,
                                                       m_params.enemyAngularVelocityMax };
            m_enemiesManager =
                std::make_shared<cannon_game::EnemiesManager>(enemyGeneratorParams,
                                                              enemyBodySpriteGeometry,
                                                              enemyBodySpriteSheet,
                                                              enemyHeadSpriteGeometry,
                                                              enemyHeadSpriteSheet);

            image = selyan::Image::create(R"(.\res\projectiles\Medium_Shell.png)");
            auto projectileTexture =
                std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            projectileTexture->textureData(image);
            delete image;
            std::shared_ptr<selyan::SpriteSheet> projectileSpriteSheet =
                std::make_shared<selyan::SpriteSheet>(projectileTexture, 1, 1);
            std::shared_ptr<selyan::SpriteGeometry> projectileSpriteGeometry =
                std::make_shared<selyan::SpriteGeometry>(
                    projectileSpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                    projectileSpriteSheet->getRelativeSpriteSize(),
                    -1);
            m_projectileManager =
                std::make_shared<cannon_game::ProjectileManager>(projectileSpriteGeometry,
                                                                 projectileSpriteSheet);

            image = selyan::Image::create(R"(.\res\player\tower2.png)");
            auto playerBodyTexture =
                std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            playerBodyTexture->textureData(image);
            delete image;
            std::shared_ptr<selyan::SpriteSheet> playerBodySpriteSheet =
                std::make_shared<selyan::SpriteSheet>(playerBodyTexture, 1, 1);
            std::shared_ptr<selyan::SpriteGeometry> playerBodySpriteGeometry =
                std::make_shared<selyan::SpriteGeometry>(
                    playerBodySpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                    playerBodySpriteSheet->getRelativeSpriteSize(),
                    -2);
            selyan::Sprite playerBodySprite(playerBodySpriteGeometry,
                                            playerBodySpriteSheet,
                                            selyan::SpriteFrame{});
            image = selyan::Image::create(R"(.\res\player\cannon2.png)");
            auto playerHeadTexture =
                std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            playerHeadTexture->textureData(image);
            delete image;
            std::shared_ptr<selyan::SpriteSheet> playerHeadSpriteSheet =
                std::make_shared<selyan::SpriteSheet>(playerHeadTexture, 1, 1);
            std::shared_ptr<selyan::SpriteGeometry> playerHeadSpriteGeometry =
                std::make_shared<selyan::SpriteGeometry>(
                    playerHeadSpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                    playerHeadSpriteSheet->getRelativeSpriteSize(),
                    -1,
                    glm::vec2(0.0, 0.25));
            selyan::Sprite playerHeadSprite(playerHeadSpriteGeometry,
                                            playerHeadSpriteSheet,
                                            selyan::SpriteFrame{});

            m_player = std::make_shared<cannon_game::Player>(playerBodySprite,
                                                             playerHeadSprite,
                                                             m_params.playerAmmoRecoveryTime,
                                                             m_params.initialPlayerAmmo);
            m_player->setPosition({ 0, 0 });
            m_player->setScale({ 1, 1 });
            m_player->setRotation(0);
            m_player->setCollision({ m_player->getPosition(), 0.5 });
            m_currentPlayerAmmo = m_params.initialPlayerAmmo;

            image = selyan::Image::create(R"(.\res\explosion\explosion-sprite-sheet.png)");
            auto explosionTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            explosionTexture->textureData(image);
            delete image;
            std::shared_ptr<selyan::SpriteSheet> explosionSpriteSheet =
                std::make_shared<selyan::SpriteSheet>(explosionTexture, 1, 8);
            std::shared_ptr<selyan::SpriteGeometry> explosionSpriteGeometry =
                std::make_shared<selyan::SpriteGeometry>(
                    explosionSpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                    explosionSpriteSheet->getRelativeSpriteSize(),
                    -0.5);
            m_explosionManager =
                std::make_shared<ExplosionManager>(explosionSpriteGeometry, explosionSpriteSheet);

            createEnemies(m_params.initialEnemyCount);
        }

        void onEvent(selyan::Event &) {}

        void onUpdate(const selyan::TimeStep &elapsedTime)
        {
            if (m_gameStop)
                return;

            if (m_currentPlayerHealth == 0)
            {
                m_loseCallback();
                m_gameStop = true;
            }

            if (m_enemiesManager->countOfAlive() == 0)
            {
                m_winCallback();
                m_gameStop = true;
            }

            if (m_currentPlayerAmmo != m_player->ammo())
            {
                m_currentPlayerAmmo = m_player->ammo();
                m_ammoChangedCallback(m_currentPlayerAmmo);
            }

            auto elapsedTimeInSeconds = elapsedTime.getSeconds();

            if (m_speedUpMode)
                elapsedTimeInSeconds *= 2;

            m_explosionManager->update(elapsedTimeInSeconds);

            m_createEnemyElapsedTime += elapsedTimeInSeconds;
            if (m_createEnemyElapsedTime >= m_params.enemyRespawnDelay)
            {
                createEnemies();
                m_createEnemyElapsedTime = 0;
            }

            if (!m_player->isDie())
                m_player->update(elapsedTimeInSeconds);

            for (auto &enemy : *m_enemiesManager)
            {
                enemy.second->update(elapsedTimeInSeconds);
            }

            //            for (auto enemyIter = m_enemiesManager->begin(); enemyIter !=
            //            m_enemiesManager->end();)
            //            {
            //                auto intersectingEnemyIter = std::find_if(
            //                    m_enemiesManager->begin(),
            //                    m_enemiesManager->end(),
            //                    [&enemyIter](const auto &other)
            //                    {
            //                        if (other.second->getUniqueId() ==
            //                        enemyIter->second->getUniqueId())
            //                            return false;
            //                        return
            //                        cannon_game::checkCollision(other.second->getCollision(),
            //                                                           enemyIter->second->getCollision());
            //                    });
            //
            //                if (intersectingEnemyIter != m_enemiesManager->end())
            //                {
            //                    auto &intersectingEnemy = intersectingEnemyIter->second;
            //                    m_explosionManager->createExplosion(intersectingEnemy->getPosition(),
            //                                                        intersectingEnemy->getRotation(),
            //                                                        glm::vec2(1, 1));
            //                    m_enemiesManager->kill(intersectingEnemyIter);
            //
            //                    m_explosionManager->createExplosion(enemyIter->second->getPosition(),
            //                                                        enemyIter->second->getRotation(),
            //                                                        glm::vec2(1, 1));
            //                    m_enemiesManager->kill(enemyIter);
            //                    break;
            //                }
            //
            //                ++enemyIter;
            //            }

            for (auto projectileIter = m_projectileManager->begin();
                 projectileIter != m_projectileManager->end();)
            {
                auto &projectile = *projectileIter;
                projectile->update(elapsedTimeInSeconds);

                if (!m_player->isDie() && m_player->getUniqueId() != projectile->getParentId())
                {
                    if (checkCollision(projectile->getCollision(), m_player->getCollision()))
                    {
                        m_explosionManager->createExplosion(
                            projectile->getPosition(),
                            projectile->getRotation(),
                            glm::vec2(0.5, 0.5) /*(*intersectingEnemy)->getScale()*/);
                        projectileIter = m_projectileManager->kill(projectileIter);
                        if (--m_currentPlayerHealth == 0)
                            m_player->die();
                        m_healthChangedCallback(m_currentPlayerHealth);
                        continue;
                    }
                }

                auto intersectingEnemy = std::find_if(
                    m_enemiesManager->begin(),
                    m_enemiesManager->end(),
                    [&projectile](const auto &enemy)
                    {
                        if (enemy.second->getUniqueId() == projectile->getParentId())
                            return false;

                        return cannon_game::checkCollision(enemy.second->getCollision(),
                                                           projectile->getCollision());
                    });

                if (intersectingEnemy != m_enemiesManager->end())
                {
                    m_explosionManager->createExplosion(intersectingEnemy->second->getPosition(),
                                                        intersectingEnemy->second->getRotation(),
                                                        glm::vec2(1, 1));
                    m_enemiesManager->kill(intersectingEnemy);

                    m_explosionManager->createExplosion(projectile->getPosition(),
                                                        projectile->getRotation(),
                                                        glm::vec2(1, 1));
                    projectileIter = m_projectileManager->kill(projectileIter);
                    continue;
                }

                ++projectileIter;
            }

            for (auto projectileIter = m_projectileManager->begin();
                 projectileIter != m_projectileManager->end();)
            {
                auto &projectile = *projectileIter;
                auto intersectingProjectileIter =
                    std::find_if(m_projectileManager->begin(),
                                 m_projectileManager->end(),
                                 [&projectile](const auto &other)
                                 {
                                     if (other->getUniqueId() == projectile->getUniqueId())
                                         return false;
                                     return cannon_game::checkCollision(other->getCollision(),
                                                                        projectile->getCollision());
                                 });

                if (intersectingProjectileIter != m_projectileManager->end())
                {
                    auto &intersectingProjectile = *intersectingProjectileIter;
                    m_explosionManager->createExplosion(intersectingProjectile->getPosition(),
                                                        intersectingProjectile->getRotation(),
                                                        glm::vec2(1, 1));
                    m_projectileManager->kill(intersectingProjectileIter);

                    m_explosionManager->createExplosion(projectile->getPosition(),
                                                        projectile->getRotation(),
                                                        glm::vec2(1, 1));
                    m_projectileManager->kill(projectileIter);
                    break;
                }

                ++projectileIter;
            }

            updatePlayer();
        }

        void onRender()
        {
            m_shader->bind();

            const float aspectRatio = utility::get_current_window_aspect_ratio();
            auto projectionMatrix = glm::ortho(-config::coordSystemWidth,
                                               config::coordSystemWidth,
                                               -config::coordSystemWidth * aspectRatio,
                                               config::coordSystemWidth * aspectRatio,
                                               0.1f,
                                               100.f);
            projectionMatrix = glm::transpose(projectionMatrix);
            m_shader->setUniform("projectionMatrix", projectionMatrix);

            auto viewMatrix = glm::identity<glm::mat4>();

            m_shader->setUniform("viewMatrix", viewMatrix);

            if (!m_player->isDie())
                m_player->draw(m_shader);

            for (auto &enemy : *m_enemiesManager)
            {
                enemy.second->draw(m_shader);
            }

            for (auto &projectile : *m_projectileManager)
            {
                projectile->draw(m_shader);
            }

            m_explosionManager->draw(m_shader);

            m_shader->unbind();
        }

        void restart()
        {
            std::cout << "Game restarting" << std::endl;

            m_gameStop = false;

            m_enemiesManager->removeAll();
            m_projectileManager->removeAll();
            m_explosionManager->removeAll();

            m_currentPlayerHealth = m_params.initialPlayerHealth;
            m_healthChangedCallback(m_currentPlayerHealth);
            m_player->reset();
            m_createEnemyElapsedTime = 0;
            createEnemies(m_params.initialEnemyCount);
        }

        void setGameParams(const cannon_game::GameParams &gameParams)
        {
            m_params = gameParams;
            m_player->setMaxAmmo(m_params.initialPlayerAmmo);
            m_enemiesManager->setEnemyGeneratorParams({ config::orbitSize,
                                                        config::minAngleDifferenceBetweenTwoEnemies,
                                                        config::minSpawnRadius,
                                                        m_params.maxSpawnRadius,
                                                        1,
                                                        m_params.enemyAngularVelocityMin,
                                                        m_params.enemyAngularVelocityMax });
        }

        void setWinCallback(const WinCallbackType &winCallback) { m_winCallback = winCallback; }

        void setLoseCallback(const LoseCallbackType &loseCallback)
        {
            m_loseCallback = loseCallback;
        }

        void setHealthChangedCallback(const HealthChangedCallbackType &healthChangedCallback)
        {
            m_healthChangedCallback = healthChangedCallback;
        }

        void setAmmoChangedCallbackType(const AmmoChangedCallbackType &ammoChangedCallback)
        {
            m_ammoChangedCallback = ammoChangedCallback;
        }

        void stop(bool value) { m_gameStop = value; }

        void speedUpMode(bool value) { m_speedUpMode = value; }

        void createEnemies(uint32_t count = 1)
        {
            for (size_t i = 0; i < count; ++i)
            {
                m_enemiesManager->createEnemy(
                    m_player->getPosition(),
                    m_params.enemyReloadTime,
                    [this](uint32_t parentId, const glm::vec2 &parentPosition, float parentRotation)
                    {
                        glm::vec2 direction =
                            glm::normalize(m_player->getPosition() - parentPosition);

                        m_projectileManager->createProjectile(parentId,
                                                              parentPosition,
                                                              parentRotation + 90,
                                                              direction,
                                                              m_params.enemyProjectileSpeed,
                                                              { 1.f, 1.f },
                                                              0.1f);
                        auto &createdProjectile = m_projectileManager->back();
                        createdProjectile->setPosition(createdProjectile->getPosition() +
                                                       direction * 1.f);
                    });
            }
        }

        void updatePlayer()
        {
            if (!m_player->hasAmmo())
                return;

            if (selyan::Input::isKeyPressed(selyan::RN_KEY_SPACE))
            {
                if (m_spaceButtonReleased)
                {
                    auto nearestProjectile = detail::findNearestProjectile(m_projectileManager,
                                                                           m_player,
                                                                           m_prevTargetId);
                    auto nearestEnemy =
                        detail::findNearestEnemy(m_enemiesManager, m_player, m_prevTargetId);
                    constexpr float angleOffset = 30;

                    if (nearestEnemy != nullptr &&
                        (nearestProjectile == nullptr ||
                         glm::length2(nearestEnemy->getPosition()) <
                             glm::length2(nearestProjectile->getPosition())))
                    {
                        m_prevTargetId = nearestEnemy->getUniqueId();

                        auto normalizeEnemyPosition =
                            glm::normalize(nearestEnemy->getPosition() - m_player->getPosition());
                        float playerRotation = glm::degrees(
                            std::atan2(normalizeEnemyPosition.x, normalizeEnemyPosition.y));

                        float nearestEnemyRotationAfterSecond = glm::radians(
                            nearestEnemy->getRotation() + nearestEnemy->getAngularVelocity());
                        glm::vec2 nearestEnemyPositionAfterSecond =
                            glm::normalize(m_player->getPosition() +
                            glm::vec2(nearestEnemy->getRadius() *
                                          glm::cos(nearestEnemyRotationAfterSecond),
                                      nearestEnemy->getRadius() *
                                          glm::sin(nearestEnemyRotationAfterSecond)));
                        float playerRotationAfterSecond = glm::degrees(
                            std::atan2(nearestEnemyPositionAfterSecond.x, nearestEnemyPositionAfterSecond.y));

                        float projectileFlyTimeInSeconds =
                            (nearestEnemy->getRadius() - 1) / m_params.playersProjectileSpeed;

                        float playerAngleDifference = playerRotationAfterSecond - playerRotation;

                        float angleOffset = playerAngleDifference * projectileFlyTimeInSeconds;

                        std::cout << "angleOffset: " << angleOffset << std::endl;

                        playerRotation +=
                            nearestEnemy->getAngularVelocity() >= 0 ? angleOffset : -angleOffset;
                        m_player->setRotation(-playerRotation);

                        glm::vec2 direction{ glm::sin(glm::radians(playerRotation)),
                                             glm::cos(glm::radians(playerRotation)) };
                        m_projectileManager->createProjectile(m_player->getUniqueId(),
                                                              m_player->getPosition() + direction,
                                                              m_player->getRotation(),
                                                              direction,
                                                              m_params.playersProjectileSpeed,
                                                              glm::vec2{ 1.f, 1.f },
                                                              0.1f);
                        // std::cout << "Rotate to enemy" << std::endl;
                        m_player->decreaseAmmo();
                    }
                    else if (nearestProjectile != nullptr)
                    {
                        m_prevTargetId = nearestProjectile->getUniqueId();

                        auto normalizeProjectilePosition = glm::normalize(
                            nearestProjectile->getPosition() - m_player->getPosition());
                        float playerRotation =
                            glm::degrees(std::atan2(normalizeProjectilePosition.x,
                                                    normalizeProjectilePosition.y));
                        m_player->setRotation(-playerRotation);
                        m_projectileManager->createProjectile(m_player->getUniqueId(),
                                                              m_player->getPosition() +
                                                                  normalizeProjectilePosition,
                                                              m_player->getRotation(),
                                                              normalizeProjectilePosition,
                                                              m_params.playersProjectileSpeed,
                                                              glm::vec2{ 1.f, 1.f },
                                                              0.1f);
                        m_player->decreaseAmmo();
                    }
                }

                m_spaceButtonReleased = false;
            }
            else
            {
                m_spaceButtonReleased = true;
            }
        }

    private:
        cannon_game::GameParams m_params;

        WinCallbackType m_winCallback;
        LoseCallbackType m_loseCallback;
        HealthChangedCallbackType m_healthChangedCallback;
        AmmoChangedCallbackType m_ammoChangedCallback;

        bool m_gameStop;
        bool m_speedUpMode;

        bool m_spaceButtonReleased = true;
        uint32_t m_prevTargetId;

        float m_createEnemyElapsedTime = 0;

        uint32_t m_currentPlayerHealth;
        uint32_t m_currentPlayerAmmo;

        std::shared_ptr<ExplosionManager> m_explosionManager;
        std::shared_ptr<cannon_game::EnemiesManager> m_enemiesManager;
        std::shared_ptr<cannon_game::ProjectileManager> m_projectileManager;

        std::shared_ptr<cannon_game::Player> m_player;

        std::shared_ptr<selyan::Shader> m_shader;
    };

    MainLayer::MainLayer(const GameParams &initParams) : pImpl(new impl(initParams)) {}

    void MainLayer::onEvent(selyan::Event &) {}

    void MainLayer::onUpdate(const selyan::TimeStep &elapsedTime) { pImpl->onUpdate(elapsedTime); }

    void MainLayer::onRender() { pImpl->onRender(); }

    void MainLayer::restart() { pImpl->restart(); }

    void MainLayer::setGameParams(const GameParams &gameParams)
    {
        pImpl->setGameParams(gameParams);
    }

    void MainLayer::setWinCallback(const MainLayer::WinCallbackType &winCallback)
    {
        pImpl->setWinCallback(winCallback);
    }

    void MainLayer::setLoseCallback(const MainLayer::LoseCallbackType &loseCallback)
    {
        pImpl->setLoseCallback(loseCallback);
    }

    void MainLayer::setHealthChangedCallback(
        const MainLayer::HealthChangedCallbackType &healthChangedCallback)
    {
        pImpl->setHealthChangedCallback(healthChangedCallback);
    }

    void MainLayer::setAmmoChangedCallbackType(
        const MainLayer::AmmoChangedCallbackType &ammoChangedCallback)
    {
        pImpl->setAmmoChangedCallbackType(ammoChangedCallback);
    }

    void MainLayer::stop(bool value) { pImpl->stop(value); }

    void MainLayer::speedUpMode(bool value) { pImpl->speedUpMode(value); }

}
