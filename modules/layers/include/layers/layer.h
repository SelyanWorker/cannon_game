#pragma once

#include "event/event.h"

namespace selyan
{
    class Layer
    {
    public:
        virtual void onEvent(Event &e) = 0;

        virtual void onUpdate() = 0;

        virtual void onRender() = 0;
    };

}
