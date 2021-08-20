#pragma once

#include "core/key_codes.h"
#include "core/time.h"
#include "event/event.h"
#include "event/key_events.h"
#include "event/mouse_event.h"
#include "event/window_events.h"
#include "core/window.h"
#include "layers/layer_stack.h"

namespace selyan
{
    class Application
    {
    private:
        void onEvent(Event &e);

        bool onWindowClose(WindowCloseEvent &e);

        void shutDown();

    public:
        Application();

        virtual ~Application();

        void pushLayer(Layer *layer);

        void popLayer();

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
        LayerStack m_layerStack;
        static Application *m_application;
    };

    Application *createApplication();

}
