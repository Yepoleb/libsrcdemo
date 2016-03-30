#include <stdint.h>
#include <algorithm>
#include <string>
#include <vector>

#include "BitBuffer.hpp"

const uint8_t MTBL[] = {0, 1, 3, 7, 15, 31, 63, 127, 255};

BitBuffer::BitBuffer(uint8_t* data, size_t size)
{
    m_buffer = data;
    m_size = size * 8; // convert to bits
}

void BitBuffer::Seek(size_t bits)
{
    m_pos += bits;
}

uint32_t BitBuffer::ReadBits(size_t bits)
{
    uint32_t ret = 0;
    size_t left = bits;

    while (left > 0)
    {
        size_t idx = m_pos >> 3; // m_pos / 8
        uint8_t bit = m_pos & 0b111; // m_pos % 8
        size_t toget = std::min((size_t)8 - bit, left);

        uint8_t nib = m_buffer[idx] >> bit & MTBL[toget];
        ret |= nib << (bits - left);

        m_pos += toget;
        left -= toget;
    }

    return ret;
}

bool BitBuffer::ReadBool()
{
    return ReadBits(1);
}

uint8_t BitBuffer::ReadU8()
{
    return static_cast<uint8_t>(ReadBits(8));
}

uint32_t BitBuffer::ReadU32()
{
    return ReadBits(32);
}

float BitBuffer::ReadFloat()
{
    // There's just no pretty way to interpret an uint32 as float
    uint32_t float_bits = ReadBits(32);
    return *reinterpret_cast<float*>(&float_bits);
}

std::string BitBuffer::ReadString()
{
    std::vector<char> temp;
    while (true)
    {
        char c = static_cast<char>(ReadBits(8));
        if (c == 0) {
            break;
        }
        temp.push_back(c);
    }
    return std::string(temp.data());
}

float BitBuffer::ReadCoord()
{
    bool hasint = ReadBool();
    bool hasfract = ReadBool();
    float value = 0;

    if (hasint || hasfract)
    {
        bool sign = ReadBool();
        if (hasint)
            value += ReadBits(14) + 1;
        if (hasfract)
            value += ReadBits(5) * (1 / 32.f);
        if (sign)
            value = -value;
    }

    return value;
}

VecCoord BitBuffer::ReadVecCoord()
{
    bool hasx = ReadBool();
    bool hasy = ReadBool();
    bool hasz = ReadBool();

    return VecCoord {
        hasx ? ReadCoord() : 0,
        hasy ? ReadCoord() : 0,
        hasz ? ReadCoord() : 0
    };
}

size_t BitBuffer::BitsLeft()
{
    return m_size - m_pos;
}