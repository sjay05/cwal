cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Local Debug Mode Preprocessor
#add_definitions(-DLOCAL_DEBUG)

set(CMAKE_CXX_FLAGS "-fmax-errors=1")

project(wal VERSION 0.0 LANGUAGES CXX)

add_library(CRC STATIC include/CRC/CRC.h)
set_property(TARGET CRC PROPERTY CXX_STANDARD 11)
set_target_properties(CRC PROPERTIES LINKER_LANGUAGE CXX)

add_library(inc STATIC src/walstream.cpp include/walstream.h 
                       src/walreader.cpp include/walreader.h
                       src/walwriter.cpp include/walwriter.h)
target_include_directories(inc PUBLIC include PUBLIC CRC)

add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC inc CRC)
