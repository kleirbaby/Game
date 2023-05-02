cmake_minimum_required(VERSION 3.4)
project(my-rpg)


include_directories(
        .
        /usr/include/SDL2/
        )

aux_source_directory(./src SRC_LIST)

link_libraries(
        -lSDL2
        -lSDL2_ttf
        -lSDL2_mixer
        -lSDL2_image
        )

set(CMAKE_CXX_STANDARD 14)

add_definitions("-Wall -g -DDEBUG")

add_executable(my-rpg ${SRC_LIST})