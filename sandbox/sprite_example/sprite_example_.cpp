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

#include "game_objects_/enemy.h"
#include "game_objects_/game_object.h"
#include "game_objects_/player.h"
#include "game_objects_/projectile.h"

#include <cmath>
#include <memory>

#include <glfw/glfw3.h>

namespace utility
{
    float get_current_window_aspect_ratio()
    {
        return float(selyan::Application::get()->getWindow()->getHeight()) /
               selyan::Application::get()->getWindow()->getWidth();
    }
}

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

        m_testSpriteSheet = std::make_shared<selyan::SpriteSheet>(testTexture, 1, 8);
        constexpr float spriteFrameLifeTime = 0.085f;
        m_testSprite =
            std::make_shared<selyan::Sprite>(m_testSpriteSheet,
                                             selyan::SpriteFrame{ 0, 0, spriteFrameLifeTime });
        //        m_testSprite->addSpriteFrame(0, 1, spriteFrameLifeTime);
        //        m_testSprite->addSpriteFrame(0, 2, spriteFrameLifeTime);
        //        m_testSprite->addSpriteFrame(0, 3, spriteFrameLifeTime);
        //        m_testSprite->addSpriteFrame(0, 4, spriteFrameLifeTime);
        //        m_testSprite->addSpriteFrame(0, 5, spriteFrameLifeTime);
        //        m_testSprite->addSpriteFrame(0, 7, spriteFrameLifeTime);

        image = selyan::Image::create(
            "D:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\player\\cannon2.png");
        auto towerTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
        towerTexture->textureData(image);
        delete image;

        m_towerSpriteSheet = std::make_shared<selyan::SpriteSheet>(towerTexture, 1, 1);
        m_towerSprite = std::make_shared<selyan::Sprite>(m_towerSpriteSheet, selyan::SpriteFrame{});

        m_player = std::make_shared<cannon_game_::Player>(0);
        m_player->setPosition({ 0, 0 });
        m_player->setScale({ 1, 1 });
        m_player->setRotation(0);
        m_player->setSprite(m_towerSprite);
        //
        //        m_enemies.push_back(
        //            std::make_shared<cannon_game_::Enemy>(1, m_player->getPosition(), 0, 50, 5,
        //            5));

        m_enemies.push_back(
            std::make_shared<cannon_game_::Enemy>(2, m_player->getPosition(), 45, -50, 6, 2));

        for (auto enemy : m_enemies)
        {
            enemy->setSprite(m_testSprite);
            enemy->setScale({ 1, 1 });
            enemy->setShootingFunction(
                [this](uint32_t parentId, const glm::vec2 &parentPosition, float parentRotation)
                {
                    //                std::cout << "parentId: " << parentId
                    //                          << " parentPosition: " <<
                    //                          glm::to_string(parentPosition) << std::endl;

                    glm::vec2 direction = glm::normalize(m_player->getPosition() - parentPosition);
                    auto found =
                        std::find_if(m_projectiles.begin(),
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
                        (*found)->setCollision({ parentPosition, 1 });
                    }
                    else
                    {
                        auto projectile =
                            std::make_shared<cannon_game_::Projectile>(10, parentId, direction, 1);
                        projectile->setSprite(m_testSprite);
                        projectile->setPosition(parentPosition);
                        projectile->setRotation(parentRotation);
                        projectile->setScale({ 1, 1 });
                        projectile->setCollision({ parentPosition, 1 });
                        m_projectiles.push_back(projectile);
                    }
                });
        }
    }

    void onEvent(selyan::Event &e) override {}

    void onUpdate(const selyan::TimeStep &timeStep) override
    {
        if (!m_player->isDie())
            m_player->update(timeStep.getSeconds());

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
        }

        for (auto &projectile : m_projectiles)
        {
            if (projectile->isDie())
                continue;

            if (!m_player->isDie())
            {
                if (checkCollision(projectile->getCollision(), m_player->getCollision()))
                {
                    // m_player->die();
                    projectile->die();
                }
            }
        }

        auto adjacentProjectile = findAdjacentProjectile();
        auto adjacentEnemy = m_enemies[0];

        constexpr float angleOffset = 30;
        if (adjacentProjectile == nullptr || glm::length2(adjacentEnemy->getPosition()) <
                                                 glm::length2(adjacentProjectile->getPosition()))
        {
            // m_player->setMTargetObject(adjacentEnemy);
            if (!adjacentEnemy->isDie())
            {
                auto normalizeEnemyPosition = glm::normalize(adjacentEnemy->getPosition());
                float playerRotation =
                    glm::degrees(std::atan2(normalizeEnemyPosition.x, normalizeEnemyPosition.y));
                playerRotation +=
                    adjacentEnemy->getMAngularVelocity() >= 0 ? -angleOffset : angleOffset;
                //                if (adjacentEnemy->getMAngularVelocity() >= 0)
                //                    playerRotation *= -1;
                m_player->setRotation(-playerRotation);
            }
        }
        else
        {
            if(!adjacentProjectile->isDie())
            {
                auto normalizeProjectilePosition =
                    glm::normalize(adjacentProjectile->getPosition());
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
        //                glm::normalize(adjacentEnemy->getPosition()); constexpr float angleOffset
        //                = 10; float playerRotation =
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
        auto projectionMatrix = glm::ortho(-n, n, -n * aspectRatio, n * aspectRatio, 0.1f, 100.f);
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
    std::shared_ptr<cannon_game_::Projectile> findAdjacentProjectile()
    {
        if (m_projectiles.empty())
            return nullptr;

        constexpr float minDistance = 0.1f;
        auto playerPosition = m_player->getPosition();
        std::shared_ptr<cannon_game_::Projectile> adjacentProjectile = m_projectiles.front();
        auto adjacentProjectileDistance =
            glm::length2(playerPosition - adjacentProjectile->getPosition());
        for (auto i = std::next(m_projectiles.begin()); i != m_projectiles.end(); ++i)
        {
            if((*i)->isDie())
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

        if(adjacentProjectile->isDie())
            return nullptr;

        return adjacentProjectile;
    }

    std::shared_ptr<cannon_game_::Enemy> findAdjacentEnemy()
    {
        if (m_enemies.empty())
            return nullptr;

        constexpr float minDistance = 0.1f;
        auto playerPosition = m_player->getPosition();
        std::shared_ptr<cannon_game_::Enemy> adjacentEnemy = m_enemies.front();
        auto adjacentEnemyDistance = glm::length2(playerPosition - adjacentEnemy->getPosition());
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

    std::shared_ptr<cannon_game_::Player> m_player;
    std::vector<std::shared_ptr<cannon_game_::Enemy>> m_enemies;
    std::vector<std::shared_ptr<cannon_game_::Projectile>> m_projectiles;

    std::shared_ptr<selyan::Sprite> m_testSprite;
    std::shared_ptr<selyan::SpriteSheet> m_testSpriteSheet;

    std::shared_ptr<selyan::Sprite> m_towerSprite;
    std::shared_ptr<selyan::SpriteSheet> m_towerSpriteSheet;

    std::shared_ptr<selyan::Sprite> m_cannon;
    std::shared_ptr<selyan::SpriteSheet> m_cannonSpriteSheet;

    selyan::Shader *m_shader;
};

int main()
{
    selyan::Application app{};

    SpriteLayer layer;
    app.pushLayer(&layer);

    app.run();

    return 0;
}
