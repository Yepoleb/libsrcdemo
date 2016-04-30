#pragma once
#include <stdint.h>
#include <string>

#include "demmessage.hpp"

class SyncTickMsg : public DemoMessage
{
public:
    SyncTickMsg(const int32_t& tick, const char* data, const size_t& data_size);
    virtual std::string toString() const;
    virtual std::string getName() const { return "SyncTickMsg"; };
    virtual MessageType getType() const { return MessageType::SyncTick; };
};
