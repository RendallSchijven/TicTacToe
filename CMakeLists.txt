cmake_minimum_required(VERSION 3.9)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 11)

add_executable(TicTacToeMc ttt_mc.cpp ttt.cpp)
add_executable(TicTacToeAb ttt_ab.cpp ttt.cpp)