#include "OGLDebug.h"
#include "application/application.h"
#include "render/shader_library.h"
#include "sprite/sprite.h"

#include "game_objects/enemy.h"
#include "game_objects/player.h"
#include "game_objects/projectile.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "game_objects_/game_object.h"

#include <memory>

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

        m_enemySpriteSheet = std::make_shared<selyan::SpriteSheet>(testTexture, 1, 8);
        constexpr float spriteFrameLifeTime = 0.085f;
        m_enemySprite =
            std::make_shared<selyan::Sprite>(m_enemySpriteSheet,
                                             selyan::SpriteFrame{ 0, 0, spriteFrameLifeTime });
        //        m_enemySprite->addSpriteFrame(0, 1, spriteFrameLifeTime);
        //        m_enemySprite->addSpriteFrame(0, 2, spriteFrameLifeTime);
        //        m_enemySprite->addSpriteFrame(0, 3, spriteFrameLifeTime);
        //        m_enemySprite->addSpriteFrame(0, 4, spriteFrameLifeTime);
        //        m_enemySprite->addSpriteFrame(0, 5, spriteFrameLifeTime);
        //        m_enemySprite->addSpriteFrame(0, 7, spriteFrameLifeTime);

        image = selyan::Image::create(
            "D:\\dev\\repos\\cannon_game\\sandbox\\sprite_example\\res\\player\\cannon2.png");
        auto towerTexture = std::shared_ptr<selyan::Texture2D>(selyan::Texture2D::create());
        towerTexture->textureData(image);
        delete image;

        m_towerSpriteSheet = std::make_shared<selyan::SpriteSheet>(towerTexture, 1, 1);
        m_towerSprite = std::make_shared<selyan::Sprite>(m_towerSpriteSheet, selyan::SpriteFrame{});

        m_player = std::make_shared<cannon_game::Player>(m_towerSprite, 0.1);


        auto enemyShootingFun = [this](const glm::vec2 &enemyPosition)
        {
            glm::vec2 direction = glm::normalize(m_player->getPosition() - enemyPosition);

            auto found = std::find_if(m_projectiles.begin(),
                                   m_projectiles.end(),
                                   [](const auto &projectile)
                                   { return projectile->isDie(); });

            cannon_game::ProjectileEntity projectileEntity{ enemyPosition + direction, 0, 1 };
            cannon_game::ProjectileMovement projectileMovement{ direction, 1 };

            if (found != m_projectiles.end())
            {
                (*found)->alive();
                (*found)->setProjectileEntity(projectileEntity);
                (*found)->setProjectileMovement(projectileMovement);
            }
            else
            {
                m_projectiles.push_back(
                    std::make_shared<cannon_game::Projectile>(m_enemySprite,
                                                              1,
                                                              projectileMovement,
                                                              projectileEntity));
            }
        };

        m_enemies.push_back(std::make_shared<cannon_game::Enemy>(m_enemySprite,
                                                                 enemyShootingFun,
                                                                 3,
                                                                 1,
                                                                 glm::vec2{ 0, 0 },
                                                                 5.,
                                                                 0,
                                                                 50));


        // m_gameObjects.push_back(std::make_unique<Enemy>(m_enemySprite, 5., 90, 100));
        // m_gameObjects.push_back(std::make_unique<Enemy>(m_enemySprite, 5., 180, 50));
        // m_gameObjects.push_back(std::make_unique<Enemy>(m_enemySprite, 5., 270, 100));

        checkError();

    }

//    ~SpriteLayer()
//    {
//        delete m_texture;
//        delete m_image;
//    }

    void onEvent(selyan::Event &e) override {}

    void onUpdate(const selyan::TimeStep &timeStep) override
    {
        for (auto &enemy : m_enemies)
        {
            if (enemy->isDie())
                continue;
            enemy->update(timeStep.getSeconds());
        }

        if (!m_player->isDie())
            m_player->update(timeStep.getSeconds());

        for (auto &projectile : m_projectiles)
        {
            if (projectile->isDie())
                continue;

            projectile->update(timeStep.getSeconds());

            if (!m_player->isDie())
            {
                auto collisionCircle = m_player->getCollisionCircle();
                if (checkCollision(projectile->getCollisionCircle(), m_player->getCollisionCircle()))
                {
                    m_player->die();
                    projectile->die();
                }
            }

            for (auto &enemy : m_enemies)
            {
                if (enemy->isDie())
                    continue;

                if (enemy->isDie())
                    continue;

                auto collisionCircle = enemy->getCollisionCircle();
                if (checkCollision(projectile->getCollisionCircle(), enemy->getCollisionCircle()))
                {
                    enemy->die();
                    projectile->die();
                }
            }
        }

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

        for (auto &enemy : m_enemies)
        {
            if (enemy->isDie())
                continue;
            enemy->draw(m_shader);
        }

        if (!m_player->isDie())
            m_player->draw(m_shader);

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
    // std::vector<std::unique_ptr<cannon_game::GameObject>> m_gameObjects;
    std::vector<std::shared_ptr<cannon_game::Enemy>> m_enemies;
    std::shared_ptr<cannon_game::Player> m_player;

    std::vector<std::shared_ptr<cannon_game::Projectile>> m_projectiles;

    std::shared_ptr<selyan::Sprite> m_enemySprite;
    std::shared_ptr<selyan::SpriteSheet> m_enemySpriteSheet;

    std::shared_ptr<selyan::Sprite> m_towerSprite;
    std::shared_ptr<selyan::SpriteSheet> m_towerSpriteSheet;

    std::shared_ptr<selyan::Sprite> m_cannon;
    std::shared_ptr<selyan::SpriteSheet> m_cannonSpriteSheet;
//
//    selyan::Texture2D *m_texture;
//    selyan::Image *m_image;

    selyan::Shader *m_shader;
};
//
// void test()
//{
//    auto modelMatrix = glm::identity<glm::mat4>();
//    //modelMatrix = glm::scale(modelMatrix, glm::vec3(100, 100, 100));
//    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
//    //modelMatrix = glm::transpose(modelMatrix);
//
//    std::cout << glm::to_string(modelMatrix) << std::endl;
//
////    float pseudoFov = 1;
////    float aspectRatio = utility::get_current_window_aspect_ratio();
////    auto projectionMatrix = glm::ortho(-pseudoFov,
////                                       pseudoFov,
////                                       -pseudoFov * aspectRatio,
////                                       pseudoFov * aspectRatio,
////                                       0.1f, 100.f);
//
//    float n = 50;
//    auto projectionMatrix = glm::ortho(-n,
//                                       n,
//                                       -n,
//                                       n);
//
//    std::cout << glm::to_string(projectionMatrix) << std::endl;
//
//    auto testMatrix = projectionMatrix * modelMatrix;
//
//    std::cout << glm::to_string(testMatrix) << std::endl;
//
//    float z = -1;
//    glm::vec4 testPos_0 = glm::vec4(0, 0, z, 1) * testMatrix;
//    glm::vec4 testPos_1 = glm::vec4(0, 1, z, 1) * testMatrix;
//    glm::vec4 testPos_2 = glm::vec4(1, 0, z, 1) * testMatrix;
//    glm::vec4 testPos_3 = glm::vec4(1, 1, z, 1) * testMatrix;
//
//    std::cout << glm::to_string(testPos_0) << std::endl;
//    std::cout << glm::to_string(testPos_1) << std::endl;
//    std::cout << glm::to_string(testPos_2) << std::endl;
//    std::cout << glm::to_string(testPos_3) << std::endl;
//
//}

int main()
{
    selyan::Application app{};

    SpriteLayer layer;
    app.pushLayer(&layer);

    app.run();

    return 0;
}
