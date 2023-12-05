cmake_minimum_required(VERSION 3.23)
project(COP3530MenInBlack)

find_package(CURL REQUIRED)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES arm64)
#set(SFML_DIR "/opt/homebrew/lib/cmake/SFML")
set(SFML_DIR "/usr/local/Cellar/sfml/2.5.1_1/lib/cmake/SFML")

add_executable(COP3530MenInBlack
        main.cpp
        graph.cpp
        graph.h
        my_kd_tree.h
        )
set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS network audio graphics window system REQUIRED)
target_link_libraries(COP3530MenInBlack PRIVATE ${COP3530MenInBlack} sfml-network sfml-audio sfml-graphics sfml-window sfml-system)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)
include_directories(SFML/include/SFML)


target_link_libraries(COP3530MenInBlack PRIVATE ${CURL_LIBRARIES})

target_include_directories(COP3530MenInBlack PRIVATE ${CURL_INCLUDE_DIRS})