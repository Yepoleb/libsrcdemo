#pragma once
#include <stdint.h>
#include <string>

#include "demmessage.hpp"
#include "common.hpp"

class SyncTickMsg : public DemoMessage
{
public:
    SyncTickMsg() : DemoMessage() { }
    SyncTickMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st);
    virtual void readBuffer(const char* data, const size_t data_size, ParserState* parser_st);
    virtual std::string toString() const;
    virtual std::string getName() const { return "SyncTickMsg"; }
    virtual MessageType getType() const { return MessageType::SYNCTICK; }
};
