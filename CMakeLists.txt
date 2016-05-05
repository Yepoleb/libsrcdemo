cmake_minimum_required (VERSION 2.8)
set (PROJECT_NAME "libsrcdemo")
set (EXEC_NAME "demoinfosrc")
project (${PROJECT_NAME})

set_target_properties (${EXEC_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features (${EXEC_NAME} PUBLIC
    cxx_defaulted_functions
    cxx_explicit_conversions
    cxx_nullptr
    cxx_range_for
    cxx_strong_enums)

if (MSVC)
    # Disable extensions to allow the alternative operator representations
    # Enable exception handling
    set (CMAKE_CXX_FLAGS "/Za /EHsc")
endif (MSVC)

include_directories (src)
include_directories (external/snappy)

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

add_executable (${EXEC_NAME} ${CPP_SOURCES})

add_subdirectory (external/snappy)
target_link_libraries (${EXEC_NAME} snappy)


