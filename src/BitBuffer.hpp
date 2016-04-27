#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "common.hpp"

class BitBuffer
{
public:
    BitBuffer(const char* data, size_t size);
    void Seek(size_t bits);
    uint32_t ReadBits(size_t bits);
    std::vector<char> ReadData(size_t bits);
    bool ReadBool();
    uint8_t ReadU8();
    uint16_t ReadU16();
    uint32_t ReadU32();
    int8_t ReadS8();
    int16_t ReadS16();
    int32_t ReadS32();
    uint32_t ReadVarU32();
    float ReadFloat();
    std::string ReadString();
    float ReadCoord();
    VecCoord ReadVecCoord();
    float ReadBitAngle(size_t bits);
    size_t BitsLeft();
    size_t GetPos() const {return m_pos;};

private:
    const char* m_buffer;
    size_t m_size;
    size_t m_pos;
};
