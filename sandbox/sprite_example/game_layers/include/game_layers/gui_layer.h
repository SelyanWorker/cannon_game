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
        //        using HealthChangedCallbackType = std::function<void(uint32_t health)>;
        //        using AmmoChangedCallbackType = std::function<void(uint32_t ammo)>;

    public:
        ImGuiLayer(const GameParams &gameParams,
                   GameParamsCallbackType gameParamsCallback,
                   bool showSettings = false)
          : m_gameParamsCallback(std::move(gameParamsCallback)),
            m_gameParams(gameParams),
            m_playerAmmo(gameParams.initialPlayerAmmo),
            m_playerHealth(3),
            m_gameStop(false),
            m_gameSpeedUp(false),
            m_showSettings(showSettings)
        {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGuiIO &io = ImGui::GetIO();

            ImGuiStyle &Style = ImGui::GetStyle();

            // Setup Dear ImGui Style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsClassic();

            // Setup Platform/Renderer bindings

            auto nativeWindow = reinterpret_cast<GLFWwindow *>(
                selyan::Application::get()->getWindow()->getNativeWindow());
            ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);
            ImGui_ImplOpenGL3_Init("#version 430");
        }

        ~ImGuiLayer()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        void onEvent(selyan::Event &e) override
        {
            // std::cout << "OnEvent" << std::endl;
        }

        void onUpdate(const selyan::TimeStep &timeStep) override {}

        void onRender() override
        {
            imguiBegin();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()!
            // You can browse its code to learn more about Dear ImGui!).
            showGameGui();

            if (m_showSettings)
                showSettings();

            if (m_showWinMessage)
                showWinMessageWindow();

            if (m_showLoseMessage)
                showLoseMessageWindow();

            //            if (show_demo_window)
            //                ImGui::ShowDemoWindow(&show_demo_window);

            imguiEnd();
        }

        void showWinMessage() { m_showWinMessage = true; }

        void showLoseMessage() { m_showLoseMessage = true; }

        void setPlayerHealth(uint32_t health) { m_playerHealth = health; }

        void setPlayerAmmo(uint32_t ammo) { m_playerAmmo = ammo; }

        void setGameRestartCallback(const GameRestartCallbackType &gameRestartCallback)
        {
            m_gameRestartCallback = gameRestartCallback;
        }
        void setGameStopCallback(const GameStopCallbackType &mGameStopCallback)
        {
            m_gameStopCallback = mGameStopCallback;
        }
        void setGameSpeedUpModeCallback(const GameSpeedUpModeCallbackType &mGameSpeedUpModeCallback)
        {
            m_gameSpeedUpModeCallback = mGameSpeedUpModeCallback;
        }

    private:
        void imguiBegin()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void imguiEnd()
        {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void showGameGui()
        {
            auto io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 200, 0));
            bool open = true;
            ImGui::Begin("game_gui",
                         &open,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoBringToFrontOnFocus);
            if (ImGui::Button("restart"))
            {
                m_showLoseMessage = false;
                m_showWinMessage = false;
                m_gameRestartCallback();
            }
            ImGui::SameLine();
            if (ImGui::Button("stop"))
            {
                m_gameStop = !m_gameStop;
                m_gameStopCallback(m_gameStop);
            }
            ImGui::SameLine();
            if (ImGui::Button("speed up"))
            {
                m_gameSpeedUp = !m_gameSpeedUp;
                m_gameSpeedUpModeCallback(m_gameSpeedUp);
            }
            ImGui::Text("Ammo: %i", m_playerAmmo);
            ImGui::SameLine();
            ImGui::Text("Health: %i", m_playerHealth);
            ImGui::End();
        }

        void showSettings()
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::Begin("game_params",
                         &m_showSettings,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground |
                             ImGuiWindowFlags_NoBringToFrontOnFocus);

            ImGui::SetNextItemWidth(100);
            ImGui::InputInt("initialEnemyCount", &m_gameParams.initialEnemyCount);
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("enemyAngularVelocityMin", &m_gameParams.enemyAngularVelocityMin);
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("enemyAngularVelocityMax", &m_gameParams.enemyAngularVelocityMax);
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("enemyRespawnDelay", &m_gameParams.enemyRespawnDelay);
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("enemyReloadTime", &m_gameParams.enemyReloadTime);
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("enemyProjectileSpeed", &m_gameParams.enemyProjectileSpeed);
            ImGui::SetNextItemWidth(100);
            ImGui::InputInt("initialPlayerAmmo", &m_gameParams.initialPlayerAmmo);
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("playerAmmoRecoveryTime", &m_gameParams.playerAmmoRecoveryTime);
            ImGui::SetNextItemWidth(100);
            ImGui::InputFloat("playersProjectileSpeed", &m_gameParams.playersProjectileSpeed);
            ImGui::SetNextItemWidth(100);
            ImGui::InputInt("initialPlayerHealth", &m_gameParams.initialPlayerHealth);

            if (ImGui::Button("Apply"))
                m_gameParamsCallback(m_gameParams);

            ImGui::End();
        }

        void showWinMessageWindow()
        {
            auto io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                                    ImGuiCond_Always,
                                    ImVec2(0.5f, 0.5f));
            ImGui::Begin("win_message",
                         &m_showSettings,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::Text("You win!");
            ImGui::End();
        }

        void showLoseMessageWindow()
        {
            auto io = ImGui::GetIO();
            ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f),
                                    ImGuiCond_Always,
                                    ImVec2(0.5f, 0.5f));
            ImGui::Begin("lose_message",
                         &m_showSettings,
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                             ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                             ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus);
            ImGui::Text("You lose!");
            ImGui::End();
        }

    private:
        GameParamsCallbackType m_gameParamsCallback;

        GameParams m_gameParams;

        GameRestartCallbackType m_gameRestartCallback;
        GameStopCallbackType m_gameStopCallback;
        GameSpeedUpModeCallbackType m_gameSpeedUpModeCallback;

        bool m_gameStop;
        bool m_gameSpeedUp;

        uint32_t m_playerAmmo;
        uint32_t m_playerHealth;

        bool show_demo_window = true;
        bool m_showSettings = true;
        bool m_showWinMessage = false;
        bool m_showLoseMessage = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    };
}
