#pragma once

#include "game_objects/enemy.h"
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

struct EnemyData
{
    float radius;
    float angle;
    float angularVelocity;
};
//
// class EnemyPositionGenerator
//{
// public:
//    EnemyPositionGenerator(float orbitSize,
//                           float spawnRadiusMax,
//                           float spawnRadiusMin,
//                           float enemyWidth,
//                           float enemyAngularVelocityMin,
//                           float enemyAngularVelocityMax)
//      : m_orbitSize(orbitSize),
//        m_orbitCount(uint32_t((spawnRadiusMax - spawnRadiusMin) / orbitSize)),
//        m_spawnRadiusMin(spawnRadiusMin),
//        m_enemyWidth(enemyWidth),
//        m_enemyAngularVelocityMin(enemyAngularVelocityMin),
//        m_enemyAngularVelocityMax(enemyAngularVelocityMax)
//    {
//    }
//
//    EnemyData generate(const std::vector<std::shared_ptr<cannon_game::Enemy>> &enemies)
//    {
//        if (enemies.empty())
//            return { m_orbitSize * m_orbitCount + m_spawnRadiusMin, 0, m_enemyAngularVelocityMin
//            };
//
//        std::srand(selyan::TimeStep::getTime().getMicro());
//
//        uint32_t orbit = 1 + std::rand() / ((RAND_MAX + 1u) / m_orbitCount);
//
//        std::vector<std::shared_ptr<cannon_game::Enemy>> enemiesOnOrbit;
//
//        std::for_each(enemies.cbegin(),
//                      enemies.cend(),
//                      [&](const std::shared_ptr<cannon_game::Enemy> &enemy)
//                      {
//                          uint32_t enemyOrbit =
//                              (enemy->getMDistanceToPlayer() - m_spawnRadiusMin) / m_orbitSize;
//                          if (enemyOrbit == orbit)
//                              enemiesOnOrbit.push_back(enemy);
//                      });
//
//        if (enemiesOnOrbit.empty())
//            return { m_orbitSize * orbit + m_spawnRadiusMin, 0, m_enemyAngularVelocityMin  };
//
//        std::sort(enemiesOnOrbit.begin(),
//                  enemiesOnOrbit.end(),
//                  [](const auto &left, const auto &right)
//                  {
//                      return left->getMDistanceToPlayer() < right->getMDistanceToPlayer();
//                  });
//
//        for (auto begin = enemiesOnOrbit.begin(); begin != std::prev(enemiesOnOrbit.end());
//        ++begin)
//        {
//            auto next = std::next(begin);
//            auto adjacentEnemyDistance =
//                glm::distance((*begin)->getPosition(), (*next)->getPosition());
//
//            if((adjacentEnemyDistance / m_enemyWidth) != 3)
//                continue;
//
//            return { m_orbitSize * orbit + m_spawnRadiusMin,
//                     ((*next)->getMLastAngle() - (*begin)->getMLastAngle()) / 2.f };
//        }
//
//        std::cout << "Can not find position for new enemy" << std::endl;
//    }
//
// private:
//    float m_orbitSize;
//    uint32_t m_orbitCount;
//    float m_spawnRadiusMin;
//    float m_enemyWidth;
//    float m_enemyAngularVelocityMin;
//    float m_enemyAngularVelocityMax;
//};

EnemyData generateRandomEnemyData(float enemyHeight,
                                  float spawnRadiusMax,
                                  float spawnRadiusMin,
                                  float enemyAngularVelocityStep,
                                  float enemyAngularVelocityMin,
                                  float enemyAngularVelocityMax)
{
    assert(enemyAngularVelocityMax - enemyAngularVelocityMin > enemyAngularVelocityStep);

    auto seed = selyan::TimeStep::getTime().getMicro();
    std::srand(seed);

    uint32_t maxOrbitCount = (spawnRadiusMax - spawnRadiusMin) / enemyHeight;
    uint32_t orbit = 1 + std::rand() / ((RAND_MAX + 1u) / maxOrbitCount);

    uint32_t angle = 0 + std::rand() / ((RAND_MAX + 1u) / 360);

    uint32_t angularVelocityStepCount =
        (enemyAngularVelocityMax - enemyAngularVelocityMin) / enemyAngularVelocityStep;
    int32_t rawAngularVelocity = 1 + std::rand() / ((RAND_MAX + 1u) / angularVelocityStepCount);
    auto angularVelocitySign = std::rand() / ((RAND_MAX + 1u));

    float angularVelocity = rawAngularVelocity * enemyAngularVelocityStep + enemyAngularVelocityMin;
    if (angularVelocitySign)
        angularVelocity *= -1;

    std::cout << rawAngularVelocity << " " << angularVelocitySign << " "
              << rawAngularVelocity * enemyAngularVelocityStep << std::endl;

    return { orbit * enemyHeight + spawnRadiusMin, float(angle), angularVelocity };
}

namespace utility
{
    float get_current_window_aspect_ratio()
    {
        return float(selyan::Application::get()->getWindow()->getHeight()) /
               selyan::Application::get()->getWindow()->getWidth();
    }
}

constexpr float enemyWidth = 1;
constexpr float spawnRadiusMax = 7;
constexpr float spawnRadiusMin = 4;
constexpr float orbitSize = 1;
constexpr float createEnemyDelay = 1;

namespace cannon_game
{
    class SpriteLayer : public selyan::Layer
    {
    public:
        SpriteLayer()
        //: m_enemyPositionGenerator(orbitSize, spawnRadiusMax, spawnRadiusMin, enemyWidth, 10, 50)
        {
            m_shader = selyan::ShaderLibrary::createShaderFromFile(
                "C:\\dev\\repos\\cannon_game\\shaders\\Sprite.glsl");

            auto image = selyan::Image::create(
                "C:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\fire-skull.png");
            auto testTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            testTexture->textureData(image);
            delete image;

            m_enemySpriteSheet = std::make_shared<selyan::SpriteSheet>(testTexture, 1, 8);
            m_enemySpriteGeometry = std::make_shared<selyan::SpriteGeometry>(
                m_enemySpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                m_enemySpriteSheet->getRelativeSpriteSize(),
                -1);
            constexpr float spriteFrameLifeTime = 0.085f;
            auto enemySprite =
                std::make_shared<selyan::Sprite>(m_enemySpriteGeometry,
                                                 m_enemySpriteSheet,
                                                 selyan::SpriteFrame{ 0, 0, spriteFrameLifeTime });
            enemySprite->addSpriteFrame(0, 1, spriteFrameLifeTime);
            enemySprite->addSpriteFrame(0, 2, spriteFrameLifeTime);
            enemySprite->addSpriteFrame(0, 3, spriteFrameLifeTime);
            enemySprite->addSpriteFrame(0, 4, spriteFrameLifeTime);
            enemySprite->addSpriteFrame(0, 5, spriteFrameLifeTime);
            enemySprite->addSpriteFrame(0, 7, spriteFrameLifeTime);
            m_sprites.push_back(enemySprite);

            image = selyan::Image::create(
                "C:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\player\\cannon2.png");
            auto towerTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
            towerTexture->textureData(image);
            delete image;

            m_towerSpriteSheet = std::make_shared<selyan::SpriteSheet>(towerTexture, 1, 1);
            m_towerSpriteGeometry = std::make_shared<selyan::SpriteGeometry>(
                m_towerSpriteSheet->applySpriteSizeAspectRation(glm::vec2{ 1, 1 }),
                m_towerSpriteSheet->getRelativeSpriteSize(),
                -1);
            auto towerSprite = std::make_shared<selyan::Sprite>(m_towerSpriteGeometry,
                                                                m_towerSpriteSheet,
                                                                selyan::SpriteFrame{});

            m_player = std::make_shared<cannon_game::Player>(0);
            m_player->setPosition({ 0, 0 });
            m_player->setScale({ 1, 1 });
            m_player->setRotation(0);
            m_player->setSprite(towerSprite);
            m_sprites.push_back(towerSprite);

            generateEnemy();
        }

        void onEvent(selyan::Event &e) override {}

        void onUpdate(const selyan::TimeStep &timeStep) override
        {
            m_createEnemyElapsedTime += timeStep.getSeconds();
            if (m_createEnemyElapsedTime >= createEnemyDelay)
            {
                // generateEnemy();

                m_createEnemyElapsedTime = 0;
            }

            if (!m_player->isDie())
                m_player->update(timeStep.getSeconds());

            for (auto &sprite : m_sprites)
            {
                sprite->update(timeStep.getSeconds());
            }

            for (auto &enemy : m_enemies)
            {
                if (enemy->isDie())
                    continue;
                enemy->update(timeStep.getSeconds());
            }

            for (auto &projectile : m_projectiles)
            {
                if (projectile->isDie())
                    continue;

                projectile->update(timeStep.getSeconds());

                if (!m_player->isDie() && m_player->getUniqueId() != projectile->getParentId())
                {
                    if (checkCollision(projectile->getCollision(), m_player->getCollision()))
                    {
                        // m_player->die();
                        projectile->die();
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
                    projectile->die();
                }

                auto intersectingEnemy = std::find_if(
                    m_enemies.begin(),
                    m_enemies.end(),
                    [&projectile](const auto &enemy)
                    {
                        if (enemy->isDie() || enemy->getUniqueId() == projectile->getParentId())
                            return false;

                        return cannon_game::checkCollision(enemy->getCollision(),
                                                           projectile->getCollision());
                    });

                if (intersectingEnemy != m_enemies.end())
                {
                    (*intersectingEnemy)->die();
                    projectile->die();
                }
            }

            updatePlayer();
        }

        void onRender() override
        {
            // std::cout << "OnRender" << std::endl;
            m_shader->bind();

            constexpr float n = 10.f;
            const float aspectRatio = utility::get_current_window_aspect_ratio();
            auto projectionMatrix =
                glm::ortho(-n, n, -n * aspectRatio, n * aspectRatio, 0.1f, 100.f);
            projectionMatrix = glm::transpose(projectionMatrix);
            m_shader->setUniform("projectionMatrix", projectionMatrix);

            auto viewMatrix = glm::identity<glm::mat4>();

            m_shader->setUniform("viewMatrix", viewMatrix);

            if (!m_player->isDie())
                m_player->draw(m_shader);

            for (auto &enemy : m_enemies)
            {
                if (enemy->isDie())
                    continue;
                enemy->draw(m_shader);
            }

            for (auto &projectile : m_projectiles)
            {
                if (projectile->isDie())
                    continue;

                projectile->draw(m_shader);
            }

            m_shader->unbind();
            checkError();
        }

    private:
        void generateProjectile(uint32_t parentId,
                                const glm::vec2 &parentPosition,
                                float parentRotation,
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
                (*found)->setPosition(parentPosition);
                (*found)->setRotation(parentRotation);
                (*found)->setDirection(direction);
                (*found)->setSpeed(speed);
                (*found)->setScale(scale);
                (*found)->setCollision({ parentPosition, 0.1f/*glm::min(scale.x, scale.y)*/ });
            }
            else
            {
                auto projectileSprite =
                    std::make_shared<selyan::Sprite>(m_enemySpriteGeometry, m_enemySpriteSheet);
                auto projectile =
                    std::make_shared<cannon_game::Projectile>(10, parentId, direction, speed);
                projectile->setSprite(projectileSprite);
                projectile->setPosition(parentPosition);
                projectile->setRotation(parentRotation);
                projectile->setScale(scale);
                projectile->setCollision({ parentPosition, 0.1f/*glm::min(scale.x, scale.y)*/ });
                m_projectiles.push_back(projectile);
            }
        }

        void generateEnemy(uint32_t count = 1)
        {
            for (size_t i = 0; i < count; ++i)
            {
                auto enemyData =
                    generateRandomEnemyData(orbitSize, spawnRadiusMax, spawnRadiusMin, 1, 45, 50);
                auto enemy = std::make_shared<cannon_game::Enemy>(2,
                                                                  m_player->getPosition(),
                                                                  enemyData.angle,
                                                                  enemyData.angularVelocity,
                                                                  enemyData.radius,
                                                                   0.5);
                auto enemySprite =
                    std::make_shared<selyan::Sprite>(m_enemySpriteGeometry,
                                                     m_enemySpriteSheet,
                                                     selyan::SpriteFrame{ 0, 0, 0.085 });
                m_sprites.push_back(enemySprite);
                enemy->setSprite(enemySprite);
                enemy->setScale({ 1, 1 });
                enemy->setCollision({ enemy->getPosition(), 0.5f });
                enemy->setShootingFunction(
                    [this](uint32_t parentId, const glm::vec2 &parentPosition, float parentRotation)
                    {
                        glm::vec2 direction =
                            glm::normalize(m_player->getPosition() - parentPosition);
//
//                        generateProjectile(parentId,
//                                           parentPosition,
//                                           parentRotation,
//                                           direction,
//                                           1.f,
//                                           { 0.6f, 0.4f });
                    });
                m_enemies.push_back(enemy);
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
            if (m_enemies.empty())
                return nullptr;

            constexpr float minDistance = 0.1f;
            auto playerPosition = m_player->getPosition();
            std::shared_ptr<cannon_game::Enemy> adjacentEnemy = m_enemies.front();
            auto adjacentEnemyDistance =
                glm::length2(playerPosition - adjacentEnemy->getPosition());
            for (auto i = std::next(m_enemies.begin()); i != m_enemies.end(); ++i)
            {
                auto projectilePosition = (*i)->getPosition();
                auto distance = glm::length2(playerPosition - projectilePosition);
                if (distance < minDistance)
                    continue;   // return?

                if (distance > adjacentEnemyDistance)
                    continue;

                adjacentEnemy = (*i);
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

                    /*if (adjacentProjectile == nullptr ||
                        glm::length2(adjacentEnemy->getPosition()) <
                            glm::length2(adjacentProjectile->getPosition()))*/
                    if (adjacentEnemy != nullptr)
                    {
                        // m_player->setMTargetObject(adjacentEnemy);
                        if (!adjacentEnemy->isDie())
                        {
                            auto normalizeEnemyPosition = glm::normalize(
                                adjacentEnemy->getPosition() - m_player->getPosition());
                            float playerRotation = glm::degrees(
                                std::atan2(normalizeEnemyPosition.x, normalizeEnemyPosition.y));
                            playerRotation += adjacentEnemy->getMAngularVelocity() >= 0 ?
                                                  -angleOffset :
                                                  angleOffset;
                            //                            if (adjacentEnemy->getMAngularVelocity()
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
                                               10,
                                               glm::vec2{ 0.5, 0.5 });
                        }
                    }
                    else
                    {
                        if (adjacentProjectile != nullptr && !adjacentProjectile->isDie())
                        {
                            auto normalizeProjectilePosition = glm::normalize(
                                adjacentProjectile->getPosition() - m_player->getPosition());
                            float playerRotation =
                                glm::degrees(std::atan2(normalizeProjectilePosition.x,
                                                        normalizeProjectilePosition.y));
                            if (adjacentEnemy->getMAngularVelocity() >= 0)
                                playerRotation *= -1;
                            m_player->setRotation(-playerRotation);
                            generateProjectile(m_player->getUniqueId(),
                                               m_player->getPosition(),
                                               m_player->getRotation(),
                                               normalizeProjectilePosition,
                                               10,
                                               glm::vec2{ 0.5, 0.5 });
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

        // EnemyPositionGenerator m_enemyPositionGenerator;

        std::shared_ptr<cannon_game::Player> m_player;
        std::vector<std::shared_ptr<cannon_game::Enemy>> m_enemies;
        std::vector<std::shared_ptr<cannon_game::Projectile>> m_projectiles;

        std::vector<std::shared_ptr<selyan::Sprite>> m_sprites;

        std::shared_ptr<selyan::SpriteSheet> m_enemySpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_enemySpriteGeometry;

        std::shared_ptr<selyan::SpriteSheet> m_towerSpriteSheet;
        std::shared_ptr<selyan::SpriteGeometry> m_towerSpriteGeometry;

        std::shared_ptr<selyan::Sprite> m_cannon;
        std::shared_ptr<selyan::SpriteSheet> m_cannonSpriteSheet;

        selyan::Shader *m_shader;
    };
}
