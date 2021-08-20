#pragma once

#include "layer.h"

#include <vector>

namespace selyan
{
    class LayerStack
    {
    public:
        LayerStack();
        virtual ~LayerStack();

        void pushLayer(Layer *layer);

        void popLayer();

        std::vector<Layer *>::iterator begin();
        std::vector<Layer *>::iterator end();

    private:
        std::vector<Layer *> m_layers;
    };

}
