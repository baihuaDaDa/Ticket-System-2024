cmake_minimum_required(VERSION 3.22)
project(Ticket-System)

set(CMAKE_CXX_STANDARD 20)
add_compile_options(-Ofast)

include_directories(src)
include_directories(db)
include_directories(lib)

add_executable(code main.cpp
        src/system/system.cpp
        src/order/order.cpp
        src/user/user.cpp
        src/train/train.cpp
        src/time/time.cpp
        src/cmdprocessor/cmdprocessor.cpp
        lib/mytools/mytools.cpp)
