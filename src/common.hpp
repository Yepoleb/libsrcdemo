#pragma once
#include <stdint.h>
#include <string>
#include <vector>

class BitBuffer;

const int32_t DEMOPROTOCOL_VERSION = 3;
const int32_t NETPROTOCOL_VERSION = 24;

const size_t MAX_EDICT_BITS = 11;

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

class STableEntry
{
public:
    STableEntry();
    STableEntry(size_t p_index, const std::string& p_name);
    STableEntry(size_t p_index, const std::string& p_name, size_t p_length, const std::vector<char>& p_data);
    void fromBuffer(size_t p_index, BitBuffer& buf);
    std::string toString() const;

    size_t index;
    std::string name;
    size_t length;
    std::vector<char> data;
};
