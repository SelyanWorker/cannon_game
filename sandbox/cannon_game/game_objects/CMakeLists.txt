add_library(cannon_game.game_objects
        STATIC
        src/game_object.cpp
        src/enemy.cpp
        src/explosion.cpp
        src/projectile.cpp
        src/player.cpp
        )
target_include_directories(cannon_game.game_objects
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )
target_link_libraries(cannon_game.game_objects
        PUBLIC
        glm
        sprite
        )
