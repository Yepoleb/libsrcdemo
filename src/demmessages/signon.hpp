#pragma once
#include <stdint.h>
#include <string>

#include "demmessage.hpp"
#include "packet.hpp"

class SignonMsg : public PacketMsg
{
public:
    SignonMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~SignonMsg() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "SignonMsg"; };
    virtual MessageType getType() const { return MessageType::SIGNON; };
};
