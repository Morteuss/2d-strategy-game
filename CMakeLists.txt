cmake_minimum_required(VERSION 3.25)
project(2d_strategy_game)

set(CMAKE_CXX_STANDARD 17)

add_executable(2d_strategy_game main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR C:/SFML-2.5.1/lib/cmake/SFML)
find_package(SFML COMPONENTS system window graphics audio network REQUIRED)
include_directories(C:/SFML-2.5.1/include)
target_link_libraries(2d_strategy_game sfml-system sfml-window sfml-graphics sfml-audio)
