#include "application/application.h"
#include "render/renderer.h"

#include <GLFW/glfw3.h>

namespace selyan
{
    uint8_t KeyStates[512];

    Application *Application::m_application = nullptr;

    Application::Application()
    {
        m_application = this;
        m_lastFrameTime = 0;
        m_run = true;

        m_window = Window::create({ "Cannon", 1024, 768 });
        // RN_APP_INFO("GLFW windows create!");
        std::cout << "GLFW windows create!" << std::endl;

        m_window->setEventFunction(std::bind(&Application::onEvent, this, std::placeholders::_1));

        for (uint16_t i = 0; i < 512; i++)
            KeyStates[i] = 0;
    }

    Application::~Application() { shutDown(); }

    void Application::pushLayer(Layer *layer) { m_layerStack.pushLayer(layer); }

    void Application::popLayer() { m_layerStack.popLayer(); }

    Application *Application::get() { return m_application; }

    Window *Application::getWindow() { return m_window; }

    float Application::getFrameTime() const { return m_lastFrameTime; }

    void Application::run()
    {
        setup();

        constexpr float BORDER_FRAME_TIME = 1.f / 120.f;

        //TimeStep time = TimeStep::getTime();

        for (; m_run;)
        {
            float time_ = TimeStep::getTime().getSeconds();
            TimeStep timeStep = time_ - m_lastFrameTime;
            m_lastFrameTime = time_;

            m_window->onUpdate();

            clear();

            for (auto layer : m_layerStack)
            {
                layer->onUpdate(timeStep);
            }

            for (auto layer : m_layerStack)
            {
                layer->onRender();
            }

            m_window->swapBuffers();
//
//            TimeStep currentTime = TimeStep::getTime();
//            m_lastFrameTime = currentTime.getSeconds() - time.getSeconds();
//            for (; m_lastFrameTime <= BORDER_FRAME_TIME;)
//            {
//                currentTime = TimeStep::getTime();
//                m_lastFrameTime = currentTime.getSeconds() - time.getSeconds();
//            }
//            time = currentTime;
        }

        shutDown();
    }

    void Application::enableCursor() { m_window->enableCursor(); }

    void Application::disableCursor() { m_window->disableCursor(); }

    void Application::onEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(
            std::bind(&Application::onWindowClose, this, std::placeholders::_1));

        for (auto layer : m_layerStack)
        {
            layer->onEvent(e);
            if (e.Handled)
                break;
        }
    }

    bool Application::onWindowClose(WindowCloseEvent &e)
    {
        m_run = false;
        return true;
    }

    /*bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        return false;
    }*/

    /*bool Application::OnKeyPressed(KeyPressedEvent& e)
    {
        if(!KeyStates[e.GetKeyCode()]) RN_APP_TRACE("Key pressed {0}", e.GetKeyCode());
        KeyStates[e.GetKeyCode()] = 1;
    }
    bool Application::OnKeyRelease(KeyReleaseEvent& e)
    {
        if (KeyStates[e.GetKeyCode()]) RN_APP_TRACE("Key release {0}", e.getKeyCode());
        KeyStates[e.getKeyCode()] = 0;
    }*/

    void Application::shutDown()
    {
        if (m_window == nullptr)
            return;

        delete m_window;
        m_window = nullptr;
        // RN_APP_INFO("Application shutdown.");
        std::cout << "Application shutdown." << std::endl;
    }
}
