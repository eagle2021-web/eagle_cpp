cmake_minimum_required(VERSION 3.26)
project(eagle_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(eagle_cpp
        main.cpp
        my_const.cpp
)
add_executable(tmp tmp.cpp)
add_executable(my_sizeof my_sizeof.cpp)
