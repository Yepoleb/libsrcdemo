#pragma once
#include <stdint.h>
#include <string>
#include <vector>

#include "demmessage.hpp"
#include "common.hpp"

class NetMsg;

class PacketMsg : public DemoMessage
{
public:
    PacketMsg() : DemoMessage() { }
    PacketMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st);
    virtual ~PacketMsg();
    virtual void readBuffer(const char* data, const size_t data_size, ParserState* parser_st);
    virtual std::string toString() const;
    virtual std::string getName() const { return "PacketMsg"; }
    virtual MessageType getType() const { return MessageType::PACKET; }

    std::vector<NetMsg*> messages;
};
