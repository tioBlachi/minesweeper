cmake_minimum_required(VERSION 3.28)
project(Minesweeper)

set(CMAKE_CXX_STANDARD 17)

# Add SFML directory path explicitly
set(SFML_DIR "/opt/homebrew/Cellar/sfml/2.6.1/lib/cmake/SFML")

find_package(SFML 2.6 REQUIRED COMPONENTS graphics audio window system)

add_executable(Minesweeper main.cpp
        Board.cpp
        Board.h
        Tile.cpp
        Tile.h
        TextureManager.cpp
        TextureManager.h
        Random.cpp
        Random.h)

target_link_libraries(Minesweeper sfml-graphics sfml-audio sfml-window sfml-system)
