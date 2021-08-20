#pragma once

#include "core/key_codes.h"
#include "core/time.h"
#include "event/event.h"
#include "event/key_events.h"
#include "event/mouse_event.h"
#include "event/window_events.h"
#include "core/window.h"

namespace selyan
{
    class Application
    {
    private:
        void onEvent(Event &e);

        bool onWindowClose(WindowCloseEvent &e);
        /*
        bool OnWindowResize(WindowResizeEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyRelease(KeyReleaseEvent& e);
        */
        void shutDown();

    public:
        Application();

        virtual ~Application();

        void run();

        static Application *get();

        Window *getWindow();

        float getFrameTime() const;

        void enableCursor();

        void disableCursor();

    private:
        bool m_run;
        Window *m_window;
        float m_frameTime;
        static Application *m_application;
    };

    Application *createApplication();

}
