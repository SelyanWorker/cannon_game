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

message(STATUS "${CMAKE_COMMAND}")
message(STATUS "${CMAKE_SOURCE_DIR}")
message(STATUS "${CMAKE_BINARY_DIR}")

add_custom_target(copy-runtime-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/sandbox/cannon_game/res ${CMAKE_BINARY_DIR}/sandbox/cannon_game/res)

add_dependencies(sb.cannon_game copy-runtime-files)