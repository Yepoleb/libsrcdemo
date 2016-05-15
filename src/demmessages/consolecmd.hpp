#pragma once
#include <stdint.h>
#include <string>

#include "demmessage.hpp"
#include "common.hpp"

class ConsoleCmdMsg : public DemoMessage
{
public:
    ConsoleCmdMsg() : DemoMessage() { }
    ConsoleCmdMsg(const int32_t tick, const char* data, const size_t data_size, ParserState* parser_st);
    virtual ~ConsoleCmdMsg() = default;
    virtual void readBuffer(const char* data, const size_t data_size, ParserState* parser_st);
    virtual std::string toString() const;
    virtual std::string getName() const { return "ConsoleCmdMsg"; }
    virtual MessageType getType() const { return MessageType::CONSOLECMD; }

    std::string command;
};
