#include <stdint.h>
#include <string>

#include "packet.hpp"
#include "signon.hpp"

SignonMsg::SignonMsg(const int32_t& tick, const char* data, const size_t& data_size) :
    PacketMsg(tick, data, data_size)
{

}

std::string SignonMsg::toString() const
{
    return PacketMsg::toString();
}
