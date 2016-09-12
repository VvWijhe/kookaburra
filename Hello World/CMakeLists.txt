cmake_minimum_required(VERSION 3.6)
project(Hello_World)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

set(SOURCE_FILES
    main.cpp)

add_executable(Hello_World ${SOURCE_FILES})