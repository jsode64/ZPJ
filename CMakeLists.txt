cmake_minimum_required(VERSION 3.16)
project(zpj)

set(CMAKE_CXX_STANDARD 23)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)

file(GLOB SOURCES "src/main.cc" "src/*.cpp")
add_executable(zpj ${SOURCES})
add_compile_options(-Wall -Wextra -Wpedantic -Werro -std=c++23)

target_link_libraries(zpj PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_mixer::SDL3_mixer)
