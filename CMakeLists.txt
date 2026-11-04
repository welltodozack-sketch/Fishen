cmake_minimum_required(VERSION 3.16)

project(FihGaem)
add_executable(Fishen src/Engine.cpp)
add_subdirectory(dependencies/raylib)

target_link_libraries(Fishen PRIVATE 
    raylib
)