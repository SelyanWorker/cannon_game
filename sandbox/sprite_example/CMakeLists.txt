add_subdirectory(game_objects)

add_executable(sb.sprite_example sprite_example.cpp)
target_link_libraries(sb.sprite_example
        PRIVATE
        render
        application
        sprite
        game_objects
        platform.ogl
        platform.glfw
        platform.stb_image
        )
