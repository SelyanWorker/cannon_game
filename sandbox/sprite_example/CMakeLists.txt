add_subdirectory(game_objects)
add_subdirectory(game_layers)

add_executable(sb.sprite_example sprite_example.cpp)
target_link_libraries(sb.sprite_example
        PRIVATE
        cannon_game.game_layers
        cannon_game.game_objects
        render
        application
        )
