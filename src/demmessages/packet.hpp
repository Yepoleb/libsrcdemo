#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "demmessage.hpp"

class NetMsg;

class PacketMsg : public DemoMessage
{
public:
    PacketMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~PacketMsg();
    virtual std::string toString() const;
    virtual std::string getName() const { return "PacketMsg"; };
    virtual MessageType getType() const { return MessageType::Packet; };

    std::vector<NetMsg*> messages;
};
