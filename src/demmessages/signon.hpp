#pragma once
#include <stdint.h>
#include <string>

#include "demmessage.hpp"
#include "packet.hpp"
#include "common.hpp"

class SignonMsg : public PacketMsg
{
public:
    SignonMsg() : PacketMsg() { }
    SignonMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st);
    virtual ~SignonMsg() = default;
    virtual std::string getName() const { return "SignonMsg"; }
    virtual MessageType getType() const { return MessageType::SIGNON; }
};
