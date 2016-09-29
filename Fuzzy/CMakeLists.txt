cmake_minimum_required(VERSION 3.6)
project(Fuzzy)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(SOURCE_FILES main.cpp fuzzy.cpp)
add_executable(Fuzzy ${SOURCE_FILES})