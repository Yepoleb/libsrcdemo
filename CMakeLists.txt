cmake_minimum_required (VERSION 2.8)
set (PROJECT_NAME "libsrcdemo")
project (${PROJECT_NAME})

set (CMAKE_CXX_FLAGS "-std=c++11")
set (CPP_SOURCE 
    src/EventParser.cpp
    src/DemoFile.cpp
    src/helpers.cpp
    src/DemoMessage.cpp
    src/BitBuffer.cpp
    src/NetMessage.cpp
    src/main.cpp
    src/VDFParser/VDFParser.cpp)

add_executable (${PROJECT_NAME} ${CPP_SOURCE})
target_link_libraries (${PROJECT_NAME} snappy)

