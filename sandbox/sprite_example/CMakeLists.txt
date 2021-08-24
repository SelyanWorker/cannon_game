add_subdirectory(game_objects)
add_subdirectory(game_layers)

add_executable(sb.sprite_example sprite_example.cpp)
target_link_libraries(sb.sprite_example
        PRIVATE
        render
        application
        sprite
        cannon_game.game_objects
        cannon_game.game_layers
        platform.ogl
        platform.glfw
        platform.stb_image
        )
