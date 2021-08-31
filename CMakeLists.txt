cmake_minimum_required(VERSION 3.7.2)

project(cannon_game VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_compile_options(-Wall -Wextra -pedantic)


# vendors
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/stb_image)
add_subdirectory(vendor/glm)

#imgui (will be removed in future)
set(imgui_path vendor/imgui)
file(GLOB imgui_src CONFIGURE_DEPENDS ${imgui_path}/*.cpp)
add_library(imgui
        STATIC
        ${imgui_src}
        ${imgui_path}/backends/imgui_impl_glfw.cpp
        ${imgui_path}/backends/imgui_impl_opengl3.cpp
        )
target_include_directories(imgui
        PUBLIC
        ${imgui_path}
        ${imgui_path}/backends
        )
target_link_libraries(imgui
        PUBLIC
        glfw
        glad
        PRIVATE
        Imm32
        )
#add_executable(imgui_test ${imgui_path}/examples/example_glfw_opengl3/main.cpp)
#target_link_libraries(imgui_test
#        PRIVATE
#        imgui
#        )

add_executable(glad_test vendor/glad/example/c++/hellowindow2.cpp)
target_link_libraries(glad_test PRIVATE
        glfw
        glad)

add_subdirectory(modules)
add_subdirectory(platforms/OpenGL)
add_subdirectory(platforms/Windows)
add_subdirectory(platforms/stb_image)

#add_library(platform OBJECT)
#target_link_libraries(platform
#        PUBLIC
#        platform.ogl
#        platform.glfw
#        platform.stb_image
#        )

add_subdirectory(sandbox)
