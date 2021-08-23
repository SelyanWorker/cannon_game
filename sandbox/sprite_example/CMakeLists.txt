add_subdirectory(game_objects)
add_subdirectory(game_objects_)

add_executable(sb.sprite_example sprite_example.cpp)
target_link_libraries(sb.sprite_example
        PRIVATE
        render
        application
        sprite
        game_objects
        game_objects_
        platform.ogl
        platform.glfw
        platform.stb_image
        )

add_executable(sb.sprite_example_ sprite_example_.cpp)
target_link_libraries(sb.sprite_example_
        PRIVATE
        render
        application
        sprite
        game_objects_
        platform.ogl
        platform.glfw
        platform.stb_image
        )
