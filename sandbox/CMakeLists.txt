add_executable(sb.basic_application basic_application/basic_application.cpp)
target_link_libraries(sb.basic_application
        PRIVATE
        render
        application
        platform.ogl
        platform.glfw
        )

add_executable(sb.imgui_example imgui_example/imgui_example.cpp)
target_link_libraries(sb.imgui_example
        PRIVATE
        render
        application
        platform.ogl
        platform.glfw
        imgui
        )

add_subdirectory(sprite_example)
