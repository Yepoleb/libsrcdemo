cmake_minimum_required (VERSION 2.8)
project (libsrcdemo)
set (EXEC_NAME "demoinfosrc")

# --- Config options ---
set (DEBUG_PACKETS OFF CACHE BOOL "Print packet after parsing")
set (DEBUG_TICKS OFF CACHE BOOL "Print tick after parsing")
set (DEFAULT_DEMO_PATH "" CACHE FILEPATH "Default demo path, intended for easier debugging")
if (DEFAULT_DEMO_PATH)
    set (ENABLE_DEFAULT_DEMO ON)
else ()
    set (ENABLE_DEFAULT_DEMO OFF)
endif ()
configure_file(src/config.hpp.in ${CMAKE_CURRENT_BINARY_DIR}/generated/config.hpp)

include_directories (src)
include_directories (external/snappy)
include_directories (external/vdfparser/include)
include_directories (${CMAKE_CURRENT_BINARY_DIR}/generated)

set (CPP_SOURCES
    src/demoinfosrc.cpp
    src/eventparser.cpp
    src/helpers.cpp
    src/soundinfo.cpp
    src/netmessage.cpp
    src/bitbuffer.cpp
    src/demofile.cpp
    src/stringtable.cpp
    src/parserstate.cpp
    src/demmessages/packet.cpp
    src/demmessages/signon.cpp
    src/demmessages/datatables.cpp
    src/demmessages/demmessage.cpp
    src/demmessages/usercmd.cpp
    src/demmessages/synctick.cpp
    src/demmessages/stringtables.cpp
    src/demmessages/consolecmd.cpp)

add_executable (${EXEC_NAME} ${CPP_SOURCES})

set_target_properties (${EXEC_NAME} PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features (${EXEC_NAME} PUBLIC
    cxx_defaulted_functions
    cxx_explicit_conversions
    cxx_nullptr
    cxx_range_for
    cxx_strong_enums
    cxx_binary_literals)

if (MSVC)
    # Disable extensions to allow the alternative operator representations
    # Enable exception handling
    set (CMAKE_CXX_FLAGS "/Za /EHsc")
endif (MSVC)

add_subdirectory (external/snappy)
add_subdirectory (external/vdfparser)
target_link_libraries (${EXEC_NAME} snappy)
target_link_libraries (${EXEC_NAME} vdfparser)

