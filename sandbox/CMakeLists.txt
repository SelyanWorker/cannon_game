add_executable(sb.basic_application basic_application/basic_application.cpp)
target_link_libraries(sb.basic_application
        PRIVATE
        render
        application
        platform.ogl
        platform.glfw
        )