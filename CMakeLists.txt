cmake_minimum_required(VERSION 3.31)
project(chess_engine)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})


set(CMAKE_CXX_STANDARD 20)



add_executable(chess_engine main.cpp
        renderer.cpp
        renderer.h
        board.cpp
        board.h
        main.h
        engine.cpp
        engine.h
)

target_link_libraries(chess_engine ${SDL2_LIBRARIES})
