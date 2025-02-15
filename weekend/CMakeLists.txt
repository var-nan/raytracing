cmake_minimum_required(VERSION 3.21)

project (RayTracing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



set(SOURCES main.cpp vec3.h ray.h)

message (STATUS "Compiler ID: " ${CMAKE_CXX_COMPILER_ID})
message (STATUS "Release flags: " ${CMAKE_CXX_FLAGS_RELEASE})
message (STATUS "Debug flags: " ${CMAKE_CXX_FLAGS_DEBUG})


add_compile_options(-Wnon-virtual-dtor)
add_compile_options(-Wreorder)
add_compile_options(-Wunused-variable)
add_compile_options(-Wall)
add_compile_options(-Wextra)


add_executable(exe ${SOURCES})
