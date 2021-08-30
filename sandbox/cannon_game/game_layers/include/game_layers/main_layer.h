#pragma once

#include "game_layers/game_params.h"
#include "game_objects/enemy.h"
#include "game_objects/explosion.h"
#include "game_objects/player.h"
#include "game_objects/projectile.h"

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

namespace cannon_game
{
    class MainLayer : public selyan::Layer
    {
    public:
        using LoseCallbackType = std::function<void()>;
        using WinCallbackType = std::function<void()>;
        using HealthChangedCallbackType = std::function<void(uint32_t)>;
        using AmmoChangedCallbackType = std::function<void(uint32_t)>;

    public:
        explicit MainLayer(const cannon_game::GameParams &initParams);

        void onEvent(selyan::Event &e) override;

        void onUpdate(const selyan::TimeStep &elapsedTime) override;

        void onRender() override;

        void restart();

        void setGameParams(const cannon_game::GameParams &gameParams);

        void setWinCallback(const WinCallbackType &winCallback);

        void setLoseCallback(const LoseCallbackType &loseCallback);

        void setHealthChangedCallback(const HealthChangedCallbackType &healthChangedCallback);

        void setAmmoChangedCallbackType(const AmmoChangedCallbackType &ammoChangedCallback);

        void stop(bool value);

        void speedUpMode(bool value);

    private:
        struct impl;
        impl* pImpl;
    };
}
