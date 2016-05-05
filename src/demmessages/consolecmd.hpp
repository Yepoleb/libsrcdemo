#pragma once
#include <stdint.h>
#include <string>

#include "demmessage.hpp"

class ConsoleCmdMsg : public DemoMessage
{
public:
    ConsoleCmdMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual ~ConsoleCmdMsg() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "ConsoleCmdMsg"; }
    virtual MessageType getType() const { return MessageType::CONSOLECMD; }

    std::string command;
};
