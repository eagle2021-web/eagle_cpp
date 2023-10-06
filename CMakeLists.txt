cmake_minimum_required(VERSION 3.5)
project(demo1)

set(CMAKE_CXX_STANDARD 14)
# 设置统一的编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
add_executable(demo1 main.cpp)
add_executable(demo2 demo2.cpp)
add_executable(server server.cpp)
add_executable(server3 server3.cpp)
add_executable(server4 server4.cpp)
add_executable(tcp_server1 tcp_server1.cpp)
add_executable(multi_server multi_server.cpp)
add_executable(multi_t_server multi_t_server.cpp)
add_executable(server_poll server_poll.cpp)

add_executable(client client.cpp)
add_executable(client2 client2.cpp)
add_executable(client3 client3.cpp)
add_executable(client4 client4.cpp)


add_executable(tmp tmp.cpp)


