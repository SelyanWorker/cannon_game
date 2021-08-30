#include "application/application.h"

#include "game_layers/gui_layer.h"
#include "game_layers/main_layer.h"

int main(int argc, char **argv)
{
    cannon_game::GameParams initialGameParams;
    cannon_game::GameConfig config("game_config.conf", initialGameParams);

    initialGameParams = config.getParams();

    selyan::Application app{};

    cannon_game::MainLayer mainLayer(initialGameParams);
    auto gameParamsCallback = [&](const cannon_game::GameParams &newParams)
    {
        if (initialGameParams.initialEnemyCount != newParams.initialEnemyCount)
            std::cout << "New initialEnemyCount: " << newParams.initialEnemyCount << std::endl;

        if (initialGameParams.enemyAngularVelocityMin != newParams.enemyAngularVelocityMin)
            std::cout << "New enemyAngularVelocityMin: " << newParams.enemyAngularVelocityMin
                      << std::endl;

        if (initialGameParams.enemyAngularVelocityMax != newParams.enemyAngularVelocityMax)
            std::cout << "New enemyAngularVelocityMax: " << newParams.enemyAngularVelocityMax
                      << std::endl;

        if (initialGameParams.enemyRespawnDelay != newParams.enemyRespawnDelay)
            std::cout << "New enemyRespawnDelay: " << newParams.enemyRespawnDelay << std::endl;

        if (initialGameParams.enemyReloadTime != newParams.enemyReloadTime)
            std::cout << "New enemyReloadTime: " << newParams.enemyReloadTime << std::endl;

        if (initialGameParams.enemyProjectileSpeed != newParams.enemyProjectileSpeed)
            std::cout << "New enemyProjectileSpeed: " << newParams.enemyProjectileSpeed
                      << std::endl;

        if (initialGameParams.initialPlayerAmmo != newParams.initialPlayerAmmo)
            std::cout << "New initialPlayerAmmo: " << newParams.initialPlayerAmmo << std::endl;

        if (initialGameParams.playerAmmoRecoveryTime != newParams.playerAmmoRecoveryTime)
            std::cout << "New playerAmmoRecoveryTime: " << newParams.playerAmmoRecoveryTime
                      << std::endl;

        if (initialGameParams.playersProjectileSpeed != newParams.playersProjectileSpeed)
            std::cout << "New playersProjectileSpeed: " << newParams.playersProjectileSpeed
                      << std::endl;

        if (initialGameParams.initialPlayerHealth != newParams.initialPlayerHealth)
            std::cout << "New playersProjectileSpeed: " << newParams.initialPlayerHealth
                      << std::endl;

        initialGameParams = newParams;

        config.setParams(newParams);
        mainLayer.setGameParams(initialGameParams);
    };

    bool showSettings = false;
    for (int i = 0; i < argc; ++i)
    {
        std::cout << argv[i] << std::endl;
        if (std::string(argv[i]) == "-s")
        {
            showSettings = true;
            break;
        }
    }

    cannon_game::ImGuiLayer guiLayer(initialGameParams, gameParamsCallback, showSettings);
    guiLayer.setGameRestartCallback(std::bind(&cannon_game::MainLayer::restart, &mainLayer));
    guiLayer.setGameStopCallback(
        std::bind(&cannon_game::MainLayer::stop, &mainLayer, std::placeholders::_1));
    guiLayer.setGameSpeedUpModeCallback(
        std::bind(&cannon_game::MainLayer::speedUpMode, &mainLayer, std::placeholders::_1));

    mainLayer.setHealthChangedCallback(
        std::bind(&cannon_game::ImGuiLayer::setPlayerHealth, &guiLayer, std::placeholders::_1));
    mainLayer.setAmmoChangedCallbackType(
        std::bind(&cannon_game::ImGuiLayer::setPlayerAmmo, &guiLayer, std::placeholders::_1));
    mainLayer.setWinCallback(std::bind(&cannon_game::ImGuiLayer::showWinMessage, &guiLayer));
    mainLayer.setLoseCallback(std::bind(&cannon_game::ImGuiLayer::showLoseMessage, &guiLayer));

    app.pushLayer(&mainLayer);
    app.pushLayer(&guiLayer);

    app.run();

    return 0;
}
