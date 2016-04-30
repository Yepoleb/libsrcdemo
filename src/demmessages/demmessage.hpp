#pragma once
#include <stdint.h>
#include <string>

#include "common.hpp"

enum class MessageType
{
    Signon = 1,
    Packet,
    SyncTick,
    ConsoleCmd,
    UserCmd,
    DataTables,
    Stop,
    StringTables
};

class DemoMessage
{
public:
    DemoMessage(const int32_t& p_tick);
    virtual ~DemoMessage() = default;
    virtual std::string toString() const;
    virtual std::string getName() const { return "Unknown DemoMessage"; };
    virtual MessageType getType() const = 0;

    int32_t tick;
};




