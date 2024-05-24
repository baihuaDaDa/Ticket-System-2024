cmake_minimum_required(VERSION 3.22)
project(Ticket-System)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3  -Wall")

include_directories(src)
include_directories(db)
include_directories(lib)

add_executable(code main.cpp
        src/system/system.cpp
        src/order/order.cpp
        src/user/user.cpp
        src/train/train.cpp)
