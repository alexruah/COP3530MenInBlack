cmake_minimum_required(VERSION 3.23)
project(COP3530MenInBlack)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(COP3530MenInBlack
        main.cpp
        my_kd_tree.h)
