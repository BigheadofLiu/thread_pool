cmake_minimum_required(VERSION 3.10)
project(ThreadPoolTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(Threadpool
    TaskQueue.cpp
    ThreadPool.cpp
    # TaskQueue.hpp
    # ThreadPool.hpp
    test.cpp 
)

# 链接 pthread 库（必须）
find_package(Threads REQUIRED)
target_link_libraries(Threadpool PRIVATE Threads::Threads)
