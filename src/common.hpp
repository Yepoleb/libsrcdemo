#pragma once
#include <stdint.h>
#include <limits>

//#define DEBUG_PACKETS
//#define DEBUG_TICKS
//#define DEM_STRINGTABLES

const int32_t DEMOPROTOCOL_VERSION = 3;
const int32_t NETPROTOCOL_VERSION = 24;

const size_t MAX_EDICT_BITS = 11;
const size_t SIZE_T_MAX = std::numeric_limits<size_t>::max();

struct QAngle
{
    float x;
    float y;
    float z;
};

struct VecCoord
{
    float x;
    float y;
    float z;
};

struct ClassInfo
{
    int32_t class_id;
    std::string tablename;
    std::string classname;
};
