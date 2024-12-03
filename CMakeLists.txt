cmake_minimum_required(VERSION 3.28)
project(2d_LD_potentional_min)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")

add_executable(2d_LD_potentional_min main.cpp
        src/Calculate/calculate.h
        src/Calculate/calculate.cpp
        src/Renderer/renderer.h
        src/Renderer/renderer.cpp
        src/Calculate/hyperparameters.cpp
        src/Calculate/hyperparameters.h)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/glm)

target_link_libraries(2d_LD_potentional_min glfw glad glm)
