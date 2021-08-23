#pragma once

#include "core/time.h"
#include "event/event.h"

namespace selyan
{
    class Layer
    {
    public:
        virtual void onEvent(Event &e) = 0;

        virtual void onUpdate(const TimeStep& timeStep) = 0;

        virtual void onRender() = 0;
    };

}
