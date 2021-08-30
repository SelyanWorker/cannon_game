#include "game_layers/gui_layer.h"

namespace cannon_game
{
    struct ImGuiLayer::impl
    {
    public:
        using GameParamsCallbackType = std::function<void(const GameParams &)>;
        using GameRestartCallbackType = std::function<void()>;
        using GameStopCallbackType = std::function<void(bool)>;
        using GameSpeedUpModeCallbackType = std::function<void(bool)>;

        impl(const GameParams &gameParams,
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

            auto nativeWindow = reinterpret_cast<GLFWwindow *>(
                selyan::Application::get()->getWindow()->getNativeWindow());
            ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);
            ImGui_ImplOpenGL3_Init("#version 430");
        }

        ~impl()
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        void onRender()
        {
            imguiBegin();

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
        void setGameStopCallback(const GameStopCallbackType &gameStopCallback)
        {
            m_gameStopCallback = gameStopCallback;
        }
        void setGameSpeedUpModeCallback(const GameSpeedUpModeCallbackType &gameSpeedUpModeCallback)
        {
            m_gameSpeedUpModeCallback = gameSpeedUpModeCallback;
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

    ImGuiLayer::ImGuiLayer(const GameParams &gameParams,
                           ImGuiLayer::GameParamsCallbackType gameParamsCallback,
                           bool showSettings)
      : pImpl(new impl(gameParams, gameParamsCallback, showSettings))
    {
    }

    void ImGuiLayer::onEvent(selyan::Event &e) {}

    void ImGuiLayer::onUpdate(const selyan::TimeStep &timeStep) {}

    void ImGuiLayer::onRender() { pImpl->onRender(); }

    void ImGuiLayer::showWinMessage() { pImpl->showWinMessage(); }

    void ImGuiLayer::showLoseMessage() { pImpl->showLoseMessage(); }

    void ImGuiLayer::setPlayerHealth(uint32_t health) { pImpl->setPlayerHealth(health); }

    void ImGuiLayer::setPlayerAmmo(uint32_t ammo) { pImpl->setPlayerAmmo(ammo); }

    void ImGuiLayer::setGameRestartCallback(
        const ImGuiLayer::GameRestartCallbackType &gameRestartCallback)
    {
        pImpl->setGameRestartCallback(gameRestartCallback);
    }

    void ImGuiLayer::setGameStopCallback(const ImGuiLayer::GameStopCallbackType &gameStopCallback)
    {
        pImpl->setGameStopCallback(gameStopCallback);
    }

    void ImGuiLayer::setGameSpeedUpModeCallback(
        const ImGuiLayer::GameSpeedUpModeCallbackType &gameSpeedUpModeCallback)
    {
        pImpl->setGameSpeedUpModeCallback(gameSpeedUpModeCallback);
    }
}
