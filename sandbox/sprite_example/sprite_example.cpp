#include "application/application.h"

#include "game_layers/main_layer.h"
#include "game_layers/gui_layer.h"

int main()
{
    selyan::Application app{};

    cannon_game::SpriteLayer mainLayer;
    app.pushLayer(&mainLayer);

    cannon_game::ImGuiLayer guiLayer;
    app.pushLayer(&guiLayer);

    app.run();

    return 0;
}
