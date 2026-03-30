cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_COMPILER "g++")
project(zpj)

set(CMAKE_CXX_STANDARD 23)
set(EXECUTABLE_NAME ${PROJECT_NAME})

find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)

file(GLOB SOURCES "src/main.cc" "src/*.cpp")
add_executable(${EXECUTABLE_NAME} ${SOURCES})
add_compile_options(-Wall -Wextra -Wpedantic -Werror -std=c++23 -g)

target_link_libraries(zpj PRIVATE SDL3::SDL3 SDL3_image::SDL3_image SDL3_mixer::SDL3_mixer SDL3_ttf::SDL3_ttf)
