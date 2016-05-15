#pragma once
#include <stdint.h>
#include <string>
#include <vector>

const int32_t DEMOPROTOCOL_VERSION = 3;
const int32_t NETPROTOCOL_VERSION = 24;

const size_t MAX_EDICT_BITS = 11;
const size_t MAX_SOUND_INDEX_BITS = 14;

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

struct Vector
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
