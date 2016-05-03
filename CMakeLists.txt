cmake_minimum_required (VERSION 2.8)
set (PROJECT_NAME "libsrcdemo")
set (EXEC_NAME "demoinfosrc")
project (${PROJECT_NAME})

set (CMAKE_CXX_FLAGS "-std=c++11")

set (CPP_SOURCES
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
    src/soundinfo.cpp
    src/netmessage.cpp
    src/bitbuffer.cpp
    src/vdfparser/vdfparser.cpp
    src/demofile.cpp)
set (SNAPPY_SOURCES
    external/snappy/snappy.cc
    external/snappy/snappy-sinksource.cc
    external/snappy/snappy-stubs-internal.cc)

include_directories(src)
include_directories(external/snappy)

add_executable (${EXEC_NAME} ${CPP_SOURCES} ${SNAPPY_SOURCES})

