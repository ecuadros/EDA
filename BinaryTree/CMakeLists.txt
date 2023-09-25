cmake_minimum_required(VERSION 3.0)
project(BinaryTree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
find_package(Threads REQUIRED)

add_executable(main main.cpp demo.cpp)
target_link_libraries(main PRIVATE Threads::Threads)