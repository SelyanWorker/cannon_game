#pragma once

#include "game_objects/enemy.h"
#include "game_objects/explosion.h"
#include "game_objects/player.h"
#include "game_objects/projectile.h"

#include "OGLDebug.h"
#include "application/application.h"
#include "core/input.h"
#include "core/key_codes.h"
#include "render/shader_library.h"
#include "sprite/sprite.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <glfw/glfw3.h>

#include <cmath>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <stack>

struct EnemyData
{
    float radius;
    float angle;
    float angularVelocity;
};

namespace config
{
    /*
    начальное количество врагов,
    минимальная и максимальная угловая скорость врага,
    время кулдауна перерождения врага,
    время кулдауна между выстрелами врага,
    скорость полёта ракет врага,
    начальный боезапас игрока,
    скорость восстановления боезапаса игрока,
    скорость полёта бомб игрока.
     */
    constexpr uint32_t initialEnemyCount = 1;
    constexpr float enemyAngularVelocityMin = 20;   // degrees
    constexpr float enemyAngularVelocityMax = 40;   // degrees
    constexpr float enemyRespawnDelay = 3;        // seconds
    constexpr float enemyReloadTime = 3.f;          // seconds
    constexpr float enemyProjectileSpeed = 3;       // parrots
    constexpr float initialPlayerAmmo = 3;
    constexpr float playerAmmoRecoveryTime = 1.f;    // seconds
    constexpr float playersProjectileSpeed = 10.f;   // parrots

    constexpr float enemyWidth = 1;
    constexpr float spawnRadiusMax = 7;
    constexpr float spawnRadiusMin = 6;
    constexpr float orbitSize = 1;

    constexpr float coordSystemWidth = 10.f;
}

// EnemyData generateRandomEnemyData(float enemyHeight,
//                                   float spawnRadiusMin,
//                                   float spawnRadiusMax,
//                                   float enemyAngularVelocityStep,
//                                   float enemyAngularVelocityMin,
//                                   float enemyAngularVelocityMax)
//{
//     assert(enemyAngularVelocityMax - enemyAngularVelocityMin > enemyAngularVelocityStep);
//
//     auto seed = selyan::TimeStep::getTime().getMicro();
//     std::srand(seed);
//
//     uint32_t maxOrbitCount = (spawnRadiusMax - spawnRadiusMin) / enemyHeight;
//     uint32_t orbit = 1 + std::rand() / ((RAND_MAX + 1u) / maxOrbitCount);
//
//     uint32_t angle = 0 + std::rand() / ((RAND_MAX + 1u) / 360);
//
//     uint32_t angularVelocityStepCount =
//         (enemyAngularVelocityMax - enemyAngularVelocityMin) / enemyAngularVelocityStep;
//     int32_t rawAngularVelocity = 1 + std::rand() / ((RAND_MAX + 1u) / angularVelocityStepCount);
//     auto angularVelocitySign = std::rand() / ((RAND_MAX + 1u));
//
//     float angularVelocity = rawAngularVelocity * enemyAngularVelocityStep +
//     enemyAngularVelocityMin; if (angularVelocitySign)
//         angularVelocity *= -1;
//
//     std::cout << rawAngularVelocity << " " << angularVelocitySign << " "
//               << rawAngularVelocity * enemyAngularVelocityStep << std::endl;
//
//     return { orbit * enemyHeight + spawnRadiusMin, float(angle), angularVelocity };
// }

namespace utility
{
    float get_current_window_aspect_ratio()
    {
        return float(selyan::Application::get()->getWindow()->getHeight()) /
               selyan::Application::get()->getWindow()->getWidth();
    }
}

namespace cannon_game
{
    class SpriteLayer : public selyan::Layer
    {
    public:
        SpriteLayer()
        {
            m_shader = selyan::ShaderLibrary::createShaderFromFile(
                "D:\\dev\\repos\\cannon_game\\shaders\\Sprite.glsl");

            auto image = selyan::Image::create(
                "D:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\enemy\\Hull_02.png");
            auto enemyBodyTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            enemyBodyTexture->textureData(image);
            delete image;
            m_enemyBodySpriteSheet = std::make_shared<selyan::SpriteSheet>(enemyBodyTexture, 1, 1);
            m_enemyBodySpriteGeometry = std::make_shared<selyan::SpriteGeometry>(
                m_enemyBodySpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                m_enemyBodySpriteSheet->getRelativeSpriteSize(),
                -2);

            image = selyan::Image::create(
                "D:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\enemy\\Gun_01.png");
            auto enemyHeadTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            enemyHeadTexture->textureData(image);
            delete image;
            m_enemyHeadSpriteSheet = std::make_shared<selyan::SpriteSheet>(enemyHeadTexture, 1, 1);
            m_enemyHeadSpriteGeometry = std::make_shared<selyan::SpriteGeometry>(
                m_enemyHeadSpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                m_enemyHeadSpriteSheet->getRelativeSpriteSize(),
                -1,
                glm::vec2{ -0.1, 0.3 });

            EnemyGeneratorParams enemyGeneratorParams{ config::orbitSize,
                                                       config::spawnRadiusMin,
                                                       config::spawnRadiusMax,
                                                       10,
                                                       config::enemyAngularVelocityMin,
                                                       config::enemyAngularVelocityMax };
            m_enemiesManager =
                std::make_shared<cannon_game::EnemiesManager>(enemyGeneratorParams,
                                                              m_enemyBodySpriteGeometry,
                                                              m_enemyBodySpriteSheet,
                                                              m_enemyHeadSpriteGeometry,
                                                              m_enemyHeadSpriteSheet);

            image = selyan::Image::create("D:\\dev\\repos\\cannon_game\\sandbox\\sprite_"
                                          "example\\res\\projectiles\\Medium_Shell.png");
            auto projectileTexture =
                std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            projectileTexture->textureData(image);
            delete image;
            m_projectileSpriteSheet =
                std::make_shared<selyan::SpriteSheet>(projectileTexture, 1, 1);
            m_projectileSpriteGeometry = std::make_shared<selyan::SpriteGeometry>(
                m_projectileSpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                m_projectileSpriteSheet->getRelativeSpriteSize(),
                -1,
                glm::vec2(-0.1f, -0.1f));

            image = selyan::Image::create(
                "D:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\player\\tower2.png");
            auto playerBodyTexture =
                std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            playerBodyTexture->textureData(image);
            delete image;
            m_playerBodySpriteSheet =
                std::make_shared<selyan::SpriteSheet>(playerBodyTexture, 1, 1);
            m_playerBodySpriteGeometry = std::make_shared<selyan::SpriteGeometry>(
                m_playerBodySpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                m_playerBodySpriteSheet->getRelativeSpriteSize(),
                -2);
            selyan::Sprite playerBodySprite(m_playerBodySpriteGeometry,
                                            m_playerBodySpriteSheet,
                                            selyan::SpriteFrame{});
            image = selyan::Image::create(
                "D:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\player\\cannon2.png");
            auto playerHeadTexture =
                std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            playerHeadTexture->textureData(image);
            delete image;
            m_playerHeadSpriteSheet =
                std::make_shared<selyan::SpriteSheet>(playerHeadTexture, 1, 1);
            m_playerHeadSpriteGeometry = std::make_shared<selyan::SpriteGeometry>(
                m_playerHeadSpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                m_playerHeadSpriteSheet->getRelativeSpriteSize(),
                -1,
                glm::vec2(0.0, 0.25));
            selyan::Sprite playerHeadSprite(m_playerHeadSpriteGeometry,
                                            m_playerHeadSpriteSheet,
                                            selyan::SpriteFrame{});

            m_player = std::make_shared<cannon_game::Player>(playerBodySprite, playerHeadSprite);
            m_player->setPosition({ 0, 0 });
            m_player->setScale({ 1, 1 });
            m_player->setRotation(0);
            // m_player->setSprite(playerHeadSprite);
            // m_sprites.push_back(playerHeadSprite);

            image = selyan::Image::create("D:\\dev\\repos\\cannon_game\\sandbox\\sprite_"
                                          "example\\res\\explosion\\explosion-sprite-sheet.png");
            auto explosionTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            explosionTexture->textureData(image);
            delete image;
            std::shared_ptr<selyan::SpriteSheet> explosionSpriteSheet = std::make_shared<selyan::SpriteSheet>(explosionTexture, 1, 8);
            std::shared_ptr<selyan::SpriteGeometry> explosionSpriteGeometry = std::make_shared<selyan::SpriteGeometry>(
                explosionSpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                explosionSpriteSheet->getRelativeSpriteSize(),
                -0.5);
            m_explosionManager = std::make_shared<ExplosionManager>(explosionSpriteGeometry,
                                                                    explosionSpriteSheet);

            // generateEnemy(config::initialEnemyCount);
            for (size_t i = 0; i < config::initialEnemyCount; ++i)
            {
                m_enemiesManager->createEnemy(
                    m_player->getPosition(),
                    config::enemyReloadTime,
                    [this](uint32_t parentId, const glm::vec2 &parentPosition, float parentRotation)
                    {
                        glm::vec2 direction =
                            glm::normalize(m_player->getPosition() - parentPosition);

                        generateProjectile(parentId,
                                           parentPosition,
                                           parentRotation + 90,
                                           direction,
                                           config::enemyProjectileSpeed,
                                           { 1.f, 1.f });
                    });
            }
        }

        void onEvent(selyan::Event &e) override {}

        void onUpdate(const selyan::TimeStep &elapsedTime) override
        {
            auto elapsedTimeInSeconds = elapsedTime.getSeconds();

            m_explosionManager->update(elapsedTimeInSeconds);

            m_createEnemyElapsedTime += elapsedTimeInSeconds;
            if (m_createEnemyElapsedTime >= config::enemyRespawnDelay)
            {
                m_enemiesManager->createEnemy(
                    m_player->getPosition(),
                    config::enemyReloadTime,
                    [this](uint32_t parentId, const glm::vec2 &parentPosition, float parentRotation)
                    {
                        glm::vec2 direction =
                            glm::normalize(m_player->getPosition() - parentPosition);

                        generateProjectile(parentId,
                                           parentPosition,
                                           parentRotation + 90,
                                           direction,
                                           config::enemyProjectileSpeed,
                                           { 1.f, 1.f });
                    });

                m_createEnemyElapsedTime = 0;
            }

            if (!m_player->isDie())
                m_player->update(elapsedTimeInSeconds);

            for (auto &enemy : *m_enemiesManager)
            {
                //                if (enemy->isDie())
                //                    continue;
                enemy.second->update(elapsedTimeInSeconds);
            }

            for (auto &projectile : m_projectiles)
            {
                if (projectile->isDie())
                    continue;

                projectile->update(elapsedTimeInSeconds);

                if (!m_player->isDie() && m_player->getUniqueId() != projectile->getParentId())
                {
                    if (checkCollision(projectile->getCollision(), m_player->getCollision()))
                    {
                        //                        m_player->die();
                        //                        m_explosionManager->createExplosion(
                        //                            m_player->getPosition(),
                        //                            m_player->getRotation(),
                        //                            glm::vec2(1, 1)
                        //                            /*(*intersectingEnemy)->getScale()*/);
                        projectile->die();
                        m_explosionManager->createExplosion(
                            projectile->getPosition(),
                            projectile->getRotation(),
                            glm::vec2(0.5, 0.5) /*(*intersectingEnemy)->getScale()*/);
                    }
                }

                auto intersectingProjectile = std::find_if(
                    m_projectiles.begin(),
                    m_projectiles.end(),
                    [&projectile](const auto &other)
                    {
                        if (other->isDie() || other->getUniqueId() == projectile->getUniqueId())
                            return false;

                        return cannon_game::checkCollision(other->getCollision(),
                                                           projectile->getCollision());
                    });

                if (intersectingProjectile != m_projectiles.end())
                {
                    (*intersectingProjectile)->die();
                    m_explosionManager->createExplosion(
                        (*intersectingProjectile)->getPosition(),
                        (*intersectingProjectile)->getRotation(),
                        glm::vec2(1, 1) /*(*intersectingEnemy)->getScale()*/);
                    projectile->die();
                    m_explosionManager->createExplosion(
                        projectile->getPosition(),
                        projectile->getRotation(),
                        glm::vec2(1, 1) /*(*intersectingEnemy)->getScale()*/);
                }

                auto intersectingEnemy = std::find_if(
                    m_enemiesManager->begin(),
                    m_enemiesManager->end(),
                    [&projectile](const auto &enemy)
                    {
                        if (enemy.second->isDie() || enemy.second->getUniqueId() == projectile->getParentId())
                            return false;

                        return cannon_game::checkCollision(enemy.second->getCollision(),
                                                           projectile->getCollision());
                    });

                if (intersectingEnemy != m_enemiesManager->end())
                {
                    m_explosionManager->createExplosion(
                        intersectingEnemy->second->getPosition(),
                        intersectingEnemy->second->getRotation(),
                        glm::vec2(1, 1) /*(*intersectingEnemy)->getScale()*/);
                    m_enemiesManager->kill(intersectingEnemy->second->getUniqueId());
                    projectile->die();
                    m_explosionManager->createExplosion(
                        projectile->getPosition(),
                        projectile->getRotation(),
                        glm::vec2(1, 1) /*(*intersectingEnemy)->getScale()*/);
                }
            }

            updatePlayer();
        }

        void onRender() override
        {
            // std::cout << "OnRender" << std::endl;
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
                //                if (enemy->isDie())
                //                    continue;
                enemy.second->draw(m_shader);
            }

            for (auto &projectile : m_projectiles)
            {
                if (projectile->isDie())
                    continue;

                projectile->draw(m_shader);
            }

            m_explosionManager->draw(m_shader);

            m_shader->unbind();
            checkError();
        }

    private:
        void generateProjectile(uint32_t parentId,
                                const glm::vec2 &position,
                                float rotation,
                                const glm::vec2 &direction,
                                float speed,
                                const glm::vec2 &scale)
        {
            auto found = std::find_if(m_projectiles.begin(),
                                      m_projectiles.end(),
                                      [](const auto &projectile) { return projectile->isDie(); });

            if (found != m_projectiles.end())
            {
                (*found)->alive();
                (*found)->setParentId(parentId);
                (*found)->setPosition(position);
                (*found)->setRotation(rotation);
                (*found)->setDirection(direction);
                (*found)->setSpeed(speed);
                (*found)->setScale(scale);
                (*found)->setCollision({ position, 0.1f /*glm::min(scale.x, scale.y)*/ });
            }
            else
            {
                selyan::Sprite projectileSprite(m_projectileSpriteGeometry,
                                                m_projectileSpriteSheet);
                auto projectile = std::make_shared<cannon_game::Projectile>(projectileSprite,
                                                                            parentId,
                                                                            direction,
                                                                            speed);
                // projectile->setSprite(projectileSprite);
                projectile->setPosition(position);
                projectile->setRotation(rotation);
                projectile->setScale(scale);
                projectile->setCollision({ position, 0.1f /*glm::min(scale.x, scale.y)*/ });
                m_projectiles.push_back(projectile);
            }
        }

        std::shared_ptr<cannon_game::Projectile> findAdjacentProjectile()
        {
            if (m_projectiles.empty())
                return nullptr;

            constexpr float minDistance = 0.1f;
            auto playerPosition = m_player->getPosition();
            std::shared_ptr<cannon_game::Projectile> adjacentProjectile = m_projectiles.front();
            auto adjacentProjectileDistance =
                glm::length2(playerPosition - adjacentProjectile->getPosition());
            for (auto i = std::next(m_projectiles.begin()); i != m_projectiles.end(); ++i)
            {
                if ((*i)->isDie() || (*i)->getParentId() == m_player->getUniqueId())
                    continue;

                auto projectilePosition = (*i)->getPosition();
                auto distance = glm::length2(playerPosition - projectilePosition);
                if (distance < minDistance)
                    continue;   // return?

                if (distance > adjacentProjectileDistance)
                    continue;

                adjacentProjectile = (*i);
                adjacentProjectileDistance = distance;
            }

            if (adjacentProjectile->isDie())
                return nullptr;

            return adjacentProjectile;
        }

        std::shared_ptr<cannon_game::Enemy> findAdjacentEnemy()
        {
            //            if (m_enemies.empty())
            //                return nullptr;
            if (m_enemiesManager->begin() == m_enemiesManager->end())
                return nullptr;

            constexpr float minDistance = 0.1f;
            auto playerPosition = m_player->getPosition();
            std::shared_ptr<cannon_game::Enemy> adjacentEnemy = m_enemiesManager->begin()->second;
            auto adjacentEnemyDistance =
                glm::length2(playerPosition - adjacentEnemy->getPosition());
            for (auto i = std::next(m_enemiesManager->begin()); i != m_enemiesManager->end(); ++i)
            {
                auto projectilePosition = i->second->getPosition();
                auto distance = glm::length2(playerPosition - projectilePosition);
                if (distance < minDistance)
                    continue;   // return?

                if (distance > adjacentEnemyDistance)
                    continue;

                adjacentEnemy = i->second;
                adjacentEnemyDistance = distance;
            }

            return adjacentEnemy;
        }

        void updatePlayer()
        {
            if (selyan::Input::isKeyPressed(selyan::RN_KEY_SPACE))
            {
                if (m_spaceButtonReleased)
                {
                    auto adjacentProjectile = findAdjacentProjectile();
                    auto adjacentEnemy = findAdjacentEnemy();
                    //
                    constexpr float angleOffset = 30;

                    if (adjacentEnemy != nullptr &&
                        (adjacentProjectile == nullptr ||
                         glm::length2(adjacentEnemy->getPosition()) <
                             glm::length2(adjacentProjectile->getPosition())))
                    {
                        // m_player->setMTargetObject(adjacentEnemy);
                        if (!adjacentEnemy->isDie())
                        {
                            auto normalizeEnemyPosition = glm::normalize(
                                adjacentEnemy->getPosition() - m_player->getPosition());
                            float playerRotation = glm::degrees(
                                std::atan2(normalizeEnemyPosition.x, normalizeEnemyPosition.y));
                            playerRotation += adjacentEnemy->getAngularVelocity() >= 0 ?
                                                  -angleOffset :
                                                  angleOffset;
                            //                            if (adjacentEnemy->getAngularVelocity()
                            //                            >= 0)
                            //                                playerRotation *= -1;
                            m_player->setRotation(-playerRotation);
                            std::cout << "player roation: " << m_player->getRotation()
                                      << ", enemy position: "
                                      << glm::to_string(adjacentEnemy->getPosition()) << std::endl;
                            generateProjectile(m_player->getUniqueId(),
                                               m_player->getPosition(),
                                               m_player->getRotation(),
                                               { glm::sin(glm::radians(playerRotation)),
                                                 glm::cos(glm::radians(playerRotation)) },
                                               config::playersProjectileSpeed,
                                               glm::vec2{ 1.f, 1.f });
                            std::cout << "Rotate to enemy" << std::endl;
                        }
                    }
                    else
                    {
                        if (!adjacentProjectile->isDie())
                        {
                            auto normalizeProjectilePosition = glm::normalize(
                                adjacentProjectile->getPosition() - m_player->getPosition());
                            float playerRotation =
                                glm::degrees(std::atan2(normalizeProjectilePosition.x,
                                                        normalizeProjectilePosition.y));
                            //                            if (adjacentEnemy->getAngularVelocity()
                            //                            >= 0)
                            //                                playerRotation *= -1;
                            m_player->setRotation(-playerRotation);
                            generateProjectile(m_player->getUniqueId(),
                                               m_player->getPosition(),
                                               m_player->getRotation(),
                                               normalizeProjectilePosition,
                                               config::playersProjectileSpeed,
                                               glm::vec2{ 1.f, 1.f });
                            std::cout << "Rotate to projectile" << std::endl;
                        }
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
        bool m_spaceButtonReleased = true;

        float m_createEnemyElapsedTime = 0;

        std::shared_ptr<ExplosionManager> m_explosionManager;

//        std::shared_ptr<selyan::SpriteSheet> m_explosionSpriteSheet;
//        std::shared_ptr<selyan::SpriteGeometry> m_explosionSpriteGeometry;

        std::shared_ptr<cannon_game::Player> m_player;
        // std::vector<std::shared_ptr<cannon_game::Enemy>> m_enemies;
        std::vector<std::shared_ptr<cannon_game::Projectile>> m_projectiles;

        std::shared_ptr<selyan::SpriteSheet> m_projectileSpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_projectileSpriteGeometry;

        std::shared_ptr<selyan::SpriteSheet> m_enemyBodySpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_enemyBodySpriteGeometry;
        std::shared_ptr<selyan::SpriteSheet> m_enemyHeadSpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_enemyHeadSpriteGeometry;
        std::shared_ptr<cannon_game::EnemiesManager> m_enemiesManager;

        std::shared_ptr<selyan::SpriteSheet> m_playerBodySpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_playerBodySpriteGeometry;
        std::shared_ptr<selyan::SpriteSheet> m_playerHeadSpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_playerHeadSpriteGeometry;

        selyan::Shader *m_shader;
    };
}
