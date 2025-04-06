cmake_minimum_required(VERSION 3.27)
project(2d-physics-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew/opt/sdl2_gfx")

find_package(SDL2 REQUIRED)
find_package(SDL2_gfx REQUIRED)

set(SDL2_IMAGE_DIR "/opt/homebrew/Cellar/sdl2_image/2.8.8/")

include_directories(
        ${PROJECT_NAME}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_DIR}/include
        ${SDL2_GFX_INCLUDE_DIRS})

file(GLOB Source "Source/*.h" "Source/*.cpp")
file(GLOB Physics "Source/Physics/*.h" "Source/Physics/*.cpp")

add_executable(${PROJECT_NAME} ${Source} ${Physics})

target_link_libraries(
        ${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_DIR}/lib/libSDL2_image.dylib
        ${SDL2_GFX_LIBRARIES})