cmake_minimum_required(VERSION 3.27)
project(2d-physics-engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB Source "Source/*.h" "Source/*.cpp")

add_executable(${PROJECT_NAME} ${Source})

#target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
#target_link_libraries(${PROJECT_NAME})