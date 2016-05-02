#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "common.hpp"

class BitBuffer
{
public:
    BitBuffer(const char* data, size_t size);
    void seek(size_t bits);
    uint32_t readBits(size_t bits);
    int32_t readSBits(size_t bits);
    std::vector<char> readData(size_t bits);
    bool readBool();
    uint8_t readU8();
    uint16_t readU16();
    uint32_t readU32();
    int8_t readS8();
    int16_t readS16();
    int32_t readS32();
    uint32_t readVarU32();
    float readFloat();
    std::string readString();
    float readCoord();
    VecCoord readVecCoord();
    float ReadBitAngle(size_t bits);
    size_t bitsLeft();
    size_t getPos() const {return m_pos;};

private:
    const char* m_buffer;
    size_t m_size;
    size_t m_pos;
};
