add_library(2d
        STATIC
        src/sprite.cpp
        src/sprite_sheet.cpp)
target_include_directories(2d
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include)