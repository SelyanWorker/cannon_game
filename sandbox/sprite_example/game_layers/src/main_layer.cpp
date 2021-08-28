#include "game_layers/main_layer.h"

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
//                              (enemy->getRadius() - m_spawnRadiusMin) / m_orbitSize;
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
//                      return left->getMDistanceToPlayer() < right->getRadius();
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
