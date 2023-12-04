cmake_minimum_required(VERSION 3.23)
project(COP3530MenInBlack)

find_package(CURL REQUIRED)


set(CMAKE_CXX_STANDARD 14)

add_executable(COP3530MenInBlack
        main.cpp
        graph.cpp
        graph.h)

target_link_libraries(COP3530MenInBlack PRIVATE ${CURL_LIBRARIES})

target_include_directories(COP3530MenInBlack PRIVATE ${CURL_INCLUDE_DIRS})

