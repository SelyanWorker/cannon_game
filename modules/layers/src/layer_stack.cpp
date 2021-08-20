#include "layers/layer_stack.h"

#include <vector>

namespace selyan
{
    LayerStack::LayerStack() { m_layers.reserve(3); }

    LayerStack::~LayerStack() {}

    void LayerStack::pushLayer(Layer *layer) { m_layers.push_back(layer); }

    void LayerStack::popLayer() { m_layers.pop_back(); }

    std::vector<Layer *>::iterator LayerStack::begin() { return m_layers.begin(); }

    std::vector<Layer *>::iterator LayerStack::end() { return m_layers.end(); }

}
