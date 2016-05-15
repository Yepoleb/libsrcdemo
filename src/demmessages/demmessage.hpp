#pragma once
#include <stdint.h>
#include <string>

#include "parserstate.hpp"
#include "common.hpp"

enum class MessageType
{
    SIGNON = 1,
    PACKET, // 2
    SYNCTICK, // 3
    CONSOLECMD, // 4
    USERCMD, // 5
    DATATABLES, // 6
    STOP, // 7
    STRINGTABLES // 8
};

class DemoMessage
{
public:
    DemoMessage();
    explicit DemoMessage(int32_t p_tick);
    virtual ~DemoMessage() = default;
    virtual void readBuffer(const char* data, const size_t data_size, ParserState* parser_st) = 0;
    virtual std::string toString() const = 0;
    virtual std::string getName() const = 0;
    virtual MessageType getType() const = 0;

    int32_t tick;
};




