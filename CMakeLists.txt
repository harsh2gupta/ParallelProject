cmake_minimum_required(VERSION 3.8)
project(ParallelProject)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        ConcurrentDeque.h
        main.cpp
        Task.h
        test.cpp
        LockFreeSteal.h)

add_executable(ParallelProject ${SOURCE_FILES})