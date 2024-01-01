cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "-fmax-errors=1")

project(wal VERSION 0.0 LANGUAGES CXX)

add_library(inc STATIC src/walstream.cpp include/walstream.h 
                       src/walreader.cpp include/walreader.h
                       src/walwriter.cpp include/walwriter.h)
target_include_directories(inc PUBLIC include)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC inc)
