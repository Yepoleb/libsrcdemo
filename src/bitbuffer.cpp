#include <stdint.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

#include "bitbuffer.hpp"

const uint8_t MTBL[] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

BitBuffer::BitBuffer(const char* data, size_t size)
{
    m_buffer = data;
    m_size = size;
    m_pos = 0;
}

void BitBuffer::seek(size_t bits)
{
    m_pos += bits;
}

uint32_t BitBuffer::readBits(size_t bits)
{
    assert(bits <= 32);
    uint32_t ret = 0;
    size_t left = bits;

    while (left > 0)
    {
        assert(m_pos < m_size);
        size_t idx = m_pos >> 3; // m_pos / 8
        uint8_t bit = m_pos & 0b111; // m_pos % 8
        size_t toget = std::min((size_t)(8 - bit), left);

        uint8_t byte = static_cast<uint8_t>(m_buffer[idx]);
        uint32_t nib = byte >> bit & MTBL[toget];
        ret |= nib << (bits - left);

        m_pos += toget;
        left -= toget;
    }

    return ret;
}

std::vector<char> BitBuffer::readData(size_t bits)
{
    std::vector<char> bytes;
    size_t left = bits;

    while (left > 0) {
        size_t toget = std::min((size_t)8, left);
        char c = static_cast<char>(readBits(toget));
        bytes.push_back(c);
        left -= toget;
    }
    return bytes;
}

bool BitBuffer::readBool()
{
    return readBits(1);
}

uint8_t BitBuffer::readU8()
{
    return static_cast<uint8_t>(readBits(8));
}

uint16_t BitBuffer::readU16()
{
    return static_cast<uint16_t>(readBits(16));
}

uint32_t BitBuffer::readU32()
{
    return readBits(32);
}

int8_t BitBuffer::readS8()
{
    return static_cast<int8_t>(readBits(8));
}

int16_t BitBuffer::readS16()
{
    return static_cast<int16_t>(readBits(16));
}

int32_t BitBuffer::readS32()
{
    return static_cast<int32_t>(readBits(32));
}

// Reads a varint encoded integer
uint32_t BitBuffer::readVarU32()
{
    uint32_t number = 0;
    for (size_t count = 0; count < sizeof(uint32_t); count++) {
        uint8_t b = readU8();
        // Add lower 7 bits to the number
        number |= (b & 0b01111111) << (7 * count);
        // Break if msb is not set
        if (not (b & 0b10000000)) {
            break;
        }
    }
    return number;
}

union UInt32FloatUnion
{
    uint32_t v_int;
    float v_float;
};

float BitBuffer::readFloat()
{
    // There's just no pretty way to interpret a uint32 as float
    uint32_t float_bits = readBits(32);
    UInt32FloatUnion ifunion;
    ifunion.v_int = float_bits;
    return ifunion.v_float;
}

std::string BitBuffer::readString()
{
    std::vector<char> temp;
    while (true)
    {
        char c = static_cast<char>(readBits(8));
        if (c == 0) {
            break;
        }
        temp.push_back(c);
    }
    return std::string(temp.data(), temp.size());
}

float BitBuffer::readCoord()
{
    bool hasint = readBool();
    bool hasfract = readBool();
    float value = 0;

    if (hasint || hasfract)
    {
        bool sign = readBool();
        if (hasint)
            value += readBits(14) + 1;
        if (hasfract)
            value += readBits(5) * (1 / 32.f);
        if (sign)
            value = -value;
    }

    return value;
}

VecCoord BitBuffer::readVecCoord()
{
    bool hasx = readBool();
    bool hasy = readBool();
    bool hasz = readBool();

    return VecCoord {
        hasx ? readCoord() : 0,
        hasy ? readCoord() : 0,
        hasz ? readCoord() : 0
    };
}

float BitBuffer::ReadBitAngle(size_t bits)
{
    float shift = (1 << bits);
    // TODO: find a better name for the variable
    uint32_t x = readBits(bits);
    float number = (float)x * (360.f / shift);
    return number;
}

size_t BitBuffer::bitsLeft()
{
    return m_size - m_pos;
}
