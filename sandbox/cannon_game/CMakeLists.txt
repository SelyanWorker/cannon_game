add_subdirectory(game_objects)
add_subdirectory(game_layers)

add_executable(sb.cannon_game main.cpp)
target_link_libraries(sb.cannon_game
        PRIVATE
        cannon_game.game_layers
        cannon_game.game_objects
        render
        application
        )
