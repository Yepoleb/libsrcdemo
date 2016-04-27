#pragma once
#include <stdint.h>
#include <limits>

//#define DEBUG_PACKETS
//#define DEBUG_TICKS

const int32_t DEMOPROTOCOL_VERSION = 3;
const int32_t NETPROTOCOL_VERSION = 24;

const size_t MAX_EDICT_BITS = 11;
const size_t WEAPON_SUBTYPE_BITS = 6;
const size_t DELTASIZE_BITS	= 20;
const size_t NET_MAX_PALYLOAD_BITS = 17;
const size_t MAX_DECAL_INDEX_BITS = 9;
const size_t SP_MODEL_INDEX_BITS = 13;
const size_t MAX_SERVER_CLASS_BITS = 9;
const size_t MAX_TABLES = 32;
const size_t EVENT_INDEX_BITS = 8;
const size_t MAX_SOUND_INDEX_BITS = 14;
const size_t MAX_EVENT_BITS = 9;

const size_t PROPINFOBITS_NUMPROPS = 10;
const size_t PROPINFOBITS_TYPE = 5;
const size_t PROPINFOBITS_NUMELEMENTS = 10;
const size_t PROPINFOBITS_NUMBITS = 7;
const size_t SPROP_NUMFLAGBITS_NETWORKED = 16;
const size_t PROPINFOBITS_FLAGS = SPROP_NUMFLAGBITS_NETWORKED;

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
