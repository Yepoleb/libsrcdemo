cmake_minimum_required (VERSION 2.8)
set (PROJECT_NAME "libsrcdemo")
set (EXEC_NAME "demoinfosrc")
project (${PROJECT_NAME})

set (CMAKE_CXX_FLAGS "-std=c++11")
set (CPP_SOURCE
    src/demoinfosrc.cpp
    src/eventparser.cpp
    src/helpers.cpp
    src/demmessages/packet.cpp
    src/demmessages/signon.cpp
    src/demmessages/datatables.cpp
    src/demmessages/demmessage.cpp
    src/demmessages/usercmd.cpp
    src/demmessages/synctick.cpp
    src/demmessages/stringtables.cpp
    src/demmessages/consolecmd.cpp
    src/netmessage.cpp
    src/bitbuffer.cpp
    src/vdfparser/vdfparser.cpp
    src/demofile.cpp)
include_directories(src)

add_executable (${EXEC_NAME} ${CPP_SOURCE})
target_link_libraries (${EXEC_NAME} snappy)

