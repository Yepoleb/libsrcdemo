#pragma once
#include <stdint.h>
#include <string>

struct VecCoord
{
    float x;
    float y;
    float z;
};

class BitBuffer
{
public:
    BitBuffer(const char* data, size_t size);
    void Seek(size_t bits);
    uint32_t ReadBits(size_t bits);
    bool ReadBool();
    uint8_t ReadU8();
    uint16_t ReadU16();
    uint32_t ReadU32();
    int8_t ReadS8();
    int16_t ReadS16();
    int32_t ReadS32();
    float ReadFloat();
    std::string ReadString();
    float ReadCoord();
    VecCoord ReadVecCoord();
    size_t BitsLeft();

private:
    const char* m_buffer;
    size_t m_size;
    size_t m_pos;
};