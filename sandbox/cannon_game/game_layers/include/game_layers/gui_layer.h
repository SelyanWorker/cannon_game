#pragma once

#include "game_params.h"

#include "application/application.h"
#include "event/event.h"
#include "layers/layer.h"

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <imgui_internal.h>
#include <iostream>

namespace cannon_game
{
    class ImGuiLayer : public selyan::Layer
    {
    public:
        using GameParamsCallbackType = std::function<void(const GameParams &)>;
        using GameRestartCallbackType = std::function<void()>;
        using GameStopCallbackType = std::function<void(bool)>;
        using GameSpeedUpModeCallbackType = std::function<void(bool)>;

    public:
        ImGuiLayer(const GameParams &gameParams,
                   GameParamsCallbackType gameParamsCallback,
                   bool showSettings = false);

        void onEvent(selyan::Event &e) override;

        void onUpdate(const selyan::TimeStep &timeStep) override;

        void onRender() override;

        void showWinMessage();

        void showLoseMessage();

        void setPlayerHealth(uint32_t health);

        void setPlayerAmmo(uint32_t ammo);

        void setGameRestartCallback(const GameRestartCallbackType &gameRestartCallback);

        void setGameStopCallback(const GameStopCallbackType &gameStopCallback);

        void setGameSpeedUpModeCallback(const GameSpeedUpModeCallbackType &gameSpeedUpModeCallback);

    private:
        struct impl;
        impl *pImpl;
    };
}
