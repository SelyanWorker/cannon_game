#pragma once

#include "game_objects/projectile.h"
#include "game_objects/enemy.h"
#include "game_objects/player.h"

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

#include <cmath>
#include <memory>
#include <iostream>

#include <glfw/glfw3.h>

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
                "D:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\fire-skull.png");
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
                "D:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\player\\cannon2.png");
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
            //
            //        m_enemies.push_back(
            //            std::make_shared<cannon_game::Enemy>(1, m_player->getPosition(), 0, 50, 5,
            //            5));

            m_enemies.push_back(
                std::make_shared<cannon_game::Enemy>(2, m_player->getPosition(), 45, -50, 6, 2));

            for (auto enemy : m_enemies)
            {
                enemy->setSprite(enemySprite);
                enemy->setScale({ 1, 1 });
                enemy->setShootingFunction(
                    [this](uint32_t parentId, const glm::vec2 &parentPosition, float parentRotation)
                    {
                        //                std::cout << "parentId: " << parentId
                        //                          << " parentPosition: " <<
                        //                          glm::to_string(parentPosition) << std::endl;

                        glm::vec2 direction =
                            glm::normalize(m_player->getPosition() - parentPosition);
                        //
                        //                    generateProjectile(parentId,
                        //                                       parentPosition,
                        //                                       parentRotation,
                        //                                       direction,
                        //                                       1.f,
                        //                                       { 0.6f, 0.4f });
                    });
            }
        }

        void onEvent(selyan::Event &e) override {}

        void onUpdate(const selyan::TimeStep &timeStep) override
        {
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
            }

            auto adjacentProjectile = findAdjacentProjectile();
            auto adjacentEnemy = m_enemies[0];

            constexpr float angleOffset = 30;
            if (adjacentProjectile == nullptr ||
                glm::length2(adjacentEnemy->getPosition()) <
                    glm::length2(adjacentProjectile->getPosition()))
            {
                // m_player->setMTargetObject(adjacentEnemy);
                if (!adjacentEnemy->isDie())
                {
                    auto normalizeEnemyPosition =
                        glm::normalize(adjacentEnemy->getPosition() - m_player->getPosition());
                    float playerRotation = glm::degrees(
                        std::atan2(normalizeEnemyPosition.x, normalizeEnemyPosition.y));
                    playerRotation +=
                        adjacentEnemy->getMAngularVelocity() >= 0 ? -angleOffset : angleOffset;
                    //                if (adjacentEnemy->getMAngularVelocity() >= 0)
                    //                    playerRotation *= -1;
                    m_player->setRotation(-playerRotation);
                    generateProjectile(m_player->getUniqueId(),
                                       m_player->getPosition(),
                                       m_player->getRotation(),
                                       { glm::cos(playerRotation), glm::sin(playerRotation) },
                                       10,
                                       glm::vec2{ 0.5, 0.5 });
                }
            }
            else
            {
                if (!adjacentProjectile->isDie())
                {
                    auto normalizeProjectilePosition =
                        glm::normalize(adjacentProjectile->getPosition() - m_player->getPosition());
                    float playerRotation = glm::degrees(
                        std::atan2(normalizeProjectilePosition.x, normalizeProjectilePosition.y));
                    //                if (adjacentEnemy->getMAngularVelocity() >= 0)
                    //                    playerRotation *= -1;
                    m_player->setRotation(-playerRotation);
                }
            }

            //        if(selyan::Input::isKeyPressed(selyan::RN_KEY_SPACE))
            //        {
            //            if(m_spaceButtonReleased)
            //            {
            //                std::cout << "Find target" << std::endl;
            //                auto adjacentProjectile = findAdjacentProjectile();
            //                auto adjacentEnemy = m_enemies[0];
            //                m_player->setMTargetObject(adjacentEnemy);
            //
            //                auto normalizeEnemyPosition =
            //                glm::normalize(adjacentEnemy->getPosition()); constexpr float
            //                angleOffset = 10; float playerRotation =
            //                    glm::degrees(std::atan2(normalizeEnemyPosition.x,
            //                    normalizeEnemyPosition.y));
            ////                if (adjacentEnemy->getMAngularVelocity() >= 0)
            ////                    playerRotation *= -1;
            //                m_player->setRotation(-(playerRotation + angleOffset));
            //            }
            //
            //            m_spaceButtonReleased = false;
            //        }
            //        else
            //        {
            //            m_spaceButtonReleased = true;
            //        }
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
                (*found)->setSpeed(1);
                (*found)->setScale(scale);
                (*found)->setCollision({ parentPosition, 1 });
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
                projectile->setCollision({ parentPosition, 1 });
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
                if ((*i)->isDie())
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

    private:
        bool m_spaceButtonReleased = true;

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
