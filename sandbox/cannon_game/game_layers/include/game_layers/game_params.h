#pragma once

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace cannon_game
{
    struct GameParams
    {
        int initialEnemyCount = 3;
        float enemyAngularVelocityMin = 20;   // degrees
        float enemyAngularVelocityMax = 25;   // degrees
        float enemyRespawnDelay = 5.f;        // seconds
        float enemyReloadTime = 7.f;          // seconds
        float enemyProjectileSpeed = 2;       // parrots
        int initialPlayerAmmo = 5;
        float playerAmmoRecoveryTime = 0.75f;   // seconds
        float playersProjectileSpeed = 4.f;     // parrots
        float maxSpawnRadius = 7;
        int initialPlayerHealth = 3;
    };

    class GameConfig
    {
    private:
        static constexpr char initialEnemyCount[] = "initialEnemyCount";
        static constexpr char enemyAngularVelocityMin[] = "enemyAngularVelocityMin";
        static constexpr char enemyAngularVelocityMax[] = "enemyAngularVelocityMax";
        static constexpr char enemyRespawnDelay[] = "enemyRespawnDelay";
        static constexpr char enemyReloadTime[] = "enemyReloadTime";
        static constexpr char enemyProjectileSpeed[] = "enemyProjectileSpeed";
        static constexpr char initialPlayerAmmo[] = "initialPlayerAmmo";
        static constexpr char playerAmmoRecoveryTime[] = "playerAmmoRecoveryTime";
        static constexpr char playersProjectileSpeed[] = "playersProjectileSpeed";
        static constexpr char initialPlayerHealth[] = "initialPlayerHealth";

    public:
        GameConfig(const char *fileName, const GameParams &defaultParams) : m_fileName(fileName)
        {
            std::fstream stream(fileName);

            if (!stream.is_open())
            {
                std::ofstream toCreateFile(fileName);
                toCreateFile.close();
                stream.open(fileName);
            }

            assert(stream.is_open());

            if (stream.peek() == std::ifstream::traits_type::eof())
            {
                stream.close();
                m_gameParams = defaultParams;
                write(defaultParams);
            }
            else
            {
                stream.close();
                m_gameParams = read();
            }
        }

        GameParams getParams() const { return m_gameParams; }

        void setParams(const GameParams &gameParams)
        {
            m_gameParams = gameParams;
            write(m_gameParams);
        }

    private:
        GameParams read()
        {
            GameParams gameParams;

            std::ifstream file(m_fileName);
            std::string name;
            file >> name >> gameParams.initialEnemyCount;
            file >> name >> gameParams.enemyAngularVelocityMin;
            file >> name >> gameParams.enemyAngularVelocityMax;
            file >> name >> gameParams.enemyRespawnDelay;
            file >> name >> gameParams.enemyReloadTime;
            file >> name >> gameParams.enemyProjectileSpeed;
            file >> name >> gameParams.initialPlayerAmmo;
            file >> name >> gameParams.playerAmmoRecoveryTime;
            file >> name >> gameParams.playersProjectileSpeed;
            file >> name >> gameParams.initialPlayerHealth;

            return gameParams;
        }

        void write(const GameParams &gameParams)
        {
            std::ofstream file(m_fileName);
            file << initialEnemyCount << " " << m_gameParams.initialEnemyCount << std::endl;
            file << enemyAngularVelocityMin << " " << m_gameParams.enemyAngularVelocityMin
                 << std::endl;
            file << enemyAngularVelocityMax << " " << m_gameParams.enemyAngularVelocityMax
                 << std::endl;
            file << enemyRespawnDelay << " " << m_gameParams.enemyRespawnDelay << std::endl;
            file << enemyReloadTime << " " << m_gameParams.enemyReloadTime << std::endl;
            file << enemyProjectileSpeed << " " << m_gameParams.enemyProjectileSpeed << std::endl;
            file << initialPlayerAmmo << " " << m_gameParams.initialPlayerAmmo << std::endl;
            file << playerAmmoRecoveryTime << " " << m_gameParams.playerAmmoRecoveryTime
                 << std::endl;
            file << playersProjectileSpeed << " " << m_gameParams.playersProjectileSpeed
                 << std::endl;
            file << initialPlayerHealth << " " << m_gameParams.initialPlayerHealth << std::endl;
        }

    private:
        GameParams m_gameParams;
        std::string m_fileName;
    };

}
