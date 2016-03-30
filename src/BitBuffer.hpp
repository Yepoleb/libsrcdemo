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
    BitBuffer(uint8_t* data, size_t size);
    void Seek(size_t bits);
    uint32_t ReadBits(size_t bits);
    bool ReadBool();
    uint8_t ReadU8();
    uint32_t ReadU32();
    float ReadFloat();
    std::string ReadString();
    float ReadCoord();
    VecCoord ReadVecCoord();
    size_t BitsLeft();

private:
    uint8_t* m_buffer;
    size_t m_size;
    size_t m_pos;
};