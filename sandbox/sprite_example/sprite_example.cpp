#include "application/application.h"

#include "game_layers/main_layer.h"

int main()
{
    selyan::Application app{};

    cannon_game::SpriteLayer layer;
    app.pushLayer(&layer);

    app.run();

    return 0;
}
