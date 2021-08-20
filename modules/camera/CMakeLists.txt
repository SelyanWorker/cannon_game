add_library(camera
        STATIC
        src/orthographic_camera.cpp
        src/perspective_camera.cpp)
target_include_directories(camera
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)