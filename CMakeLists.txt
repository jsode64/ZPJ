cmake_minimum_required(VERSION 3.16)
project(zpj CXX)

set(CMAKE_CXX_STANDARD 23)
set(EXECUTABLE_NAME ${PROJECT_NAME})

include(FetchContent)

FetchContent_Declare(
    SDL
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.8
)

FetchContent_Declare(
    SDL_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-3.4.4
)

FetchContent_Declare(
    SDL_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG release-3.2.0
)

FetchContent_Declare(
    SDL_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-3.2.2
)

FetchContent_MakeAvailable(SDL SDL_image SDL_mixer SDL_ttf)

include(GNUInstallDirs)

configure_file(
    ${CMAKE_SOURCE_DIR}/src/config.hpp.in
    ${CMAKE_BINARY_DIR}/src/config.hpp
)

file(GLOB SOURCES "src/main.cc" "src/*.cpp")
add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_compile_options(${EXECUTABLE_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Werror -g
)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}/src
)

install(TARGETS ${EXECUTABLE_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/ZPJ
)